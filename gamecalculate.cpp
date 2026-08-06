#include "common/macros.h"
#include "gameroot.h"
#include "logic/gamelogic/core/gamecalculate.h"
#include "utils/tarslog.h"
#include "context/context.h"
#include "common/nndef.h"
#include "common/nnlogic.h"
#include "config/gameconfig.h"
#include "message/sendroommessage.h"

using namespace nndef;

namespace game
{
    namespace logic
    {
        namespace gamelogic
        {
            void GameCalculate(GameRoot *root)
            {
                PERFSTATS_ENTRY();
                __TRY__

                using namespace context;
                using namespace nnlogic;
                using namespace config;
                using namespace RoomSo;
                using namespace message;

                //step1 cal user total bet and compare
                std::vector<long> vUserBetTotal;
                std::map<int, long> mUserBetTotal;
                std::vector<cid_t> vCompareCids; //all compare cids <
                std::map<cid_t, User> &usermap = root->con->refUserMap();
                for (auto it = usermap.begin(); it != usermap.end(); it++)
                {
                    long totalBetNum = it->second.getRoundBetNum(-1);
                    vUserBetTotal.push_back(totalBetNum);
                    mUserBetTotal.insert(std::make_pair(it->first, totalBetNum));

                    User* user = root->con->getUserByCid(it->first);
                    if(user && !user->isFold() && !user->isMidSit())
                    {
                        vCompareCids.push_back(it->first);
                    }
                }
                std::sort(vUserBetTotal.begin(), vUserBetTotal.end());

                //比牌
                for(auto cid : vCompareCids)
                {
                    User* user = root->con->getUserByCid(cid);
                    if(user && !user->isFold() && !user->isMidSit())
                    {
                        E_NN_TYPE nnType = nnlogic::getnn(user->getVecCards());
                        user->setCardType(nnType);
                    }
                }

                std::sort(vCompareCids.begin(), vCompareCids.end(), [root](cid_t lcid, cid_t rcid)->bool{
                    User* luser = root->con->getUserByCid(lcid);
                    User* ruser = root->con->getUserByCid(rcid);
                    if(luser->getCardType() != ruser->getCardType())
                    {
                        return luser->getCardType() < ruser->getCardType();
                    }
                    else
                    {
                        return nnlogic::compare(luser->getVecCards(), ruser->getVecCards(), luser->getCardType());
                    }
                });

                //step2 cal pool base score
                std::vector<long> vPoolBaseScore;
                if(vUserBetTotal.size() > 1)
                {
                    vPoolBaseScore.push_back(vUserBetTotal[0]);
                    for(unsigned int i= 1; i< vUserBetTotal.size(); i++)
                    {
                        if(vUserBetTotal[i] != vUserBetTotal[i-1])
                        {
                            vPoolBaseScore.push_back(vUserBetTotal[i] - vUserBetTotal[i-1]);
                        }
                    } 
                }

                //step3 cal pool link user and pool num
                std::map<int, std::vector<cid_t>> mPoolCids; //pool_id: cids
                std::map<int, long> mPoolNum; //pool_id: num
                int pool_index = 0;
                for(auto basescore : vPoolBaseScore)
                {
                    pool_index++;
                    long indexPoolNum = 0;
                    std::vector<cid_t> vPoolCids;
                    for(auto& item : mUserBetTotal)
                    {
                        if (item.second >= basescore)
                        {
                            indexPoolNum += basescore;
                            item.second -= basescore;

                            User* user = root->con->getUserByCid(item.first);
                            if(user && !user->isFold() && !user->isMidSit())
                            {
                                vPoolCids.push_back(item.first);
                            }
                        }
                    }
                    mPoolNum.insert(std::make_pair(pool_index, indexPoolNum));
                    mPoolCids.insert(std::make_pair(pool_index, vPoolCids));
                }
                
                for(auto it : mPoolNum)
                {
                    DLOG_TRACE("roomid:" << root->roomid() <<"game cal pool_index: "<< it.first <<" , nmum: "<< it.second); 
                }

                //step4 compair and win score
                for(auto pool_cid : mPoolCids)
                {   
                    auto cids = pool_cid.second;
                    for(int i = vCompareCids.size() - 1; i >= 0; i--)
                    {
                        auto cur_cid = vCompareCids[i];
                        auto it = std::find_if(cids.begin(), cids.end(), [cur_cid](cid_t cid)->bool{
                            return cur_cid == cid;
                        });
                        if(it != cids.end())//赢家
                        {
                            User* winuser = root->con->getUserByCid(*it);
                            auto itt = mPoolNum.find(pool_cid.first);
                            if(itt != mPoolNum.end() && winuser)
                            {
                                winuser->setChangeNum(winuser->getChangeNum() + itt->second);
                                DLOG_TRACE("roomid:" << root->roomid() <<"game cal cid: "<< *it <<" , uid: "<< winuser->getUid()<<", changenum:"<<winuser->getChangeNum()); 
                            }
                            break;
                        }
                    }
                }

                //扣税
                for (auto it = usermap.begin(); it != usermap.end(); it++)
                {
                    it->second.setProfitNum(0);
                    if(it->second.getChangeNum() > 0)
                    {
                        long addnum = it->second.getChangeNum() - it->second.getRoundBetNum(-1);
                        if(addnum <= 0 )
                        {
                            DLOG_TRACE("roomid:" << root->roomid() <<"game cal err cid: "<< it->first <<" , uid: "<< it->second.getUid()<<", addnum:"<<addnum);
                            continue;
                        }
                        it->second.setProfitNum(std::floor(addnum * root->cfg->getSysProfit() / 100 ));
                        it->second.setChangeNum(it->second.getChangeNum() - it->second.getProfitNum());
                        it->second.setSHWealth(it->second.getSHWealth() + it->second.getChangeNum());
                        DLOG_TRACE("roomid:" << root->roomid() <<"game cal cid: "<< it->first  <<" , uid: "<< it->second.getUid()<<", changenum:"<<it->second.getChangeNum()); 
                        
                        TGAME_PlayerScoreChange pmm;
                        pmm.changeType = 99;
                        pmm.changePoint.insert(std::make_pair(it->second.getUid(), it->second.getProfitNum()));
                        sendRoomMessage<TGAME_PlayerScoreChange>(TGAME_PlayerScoreChange_E, pmm, root);
                    }
                }
                __CATCH__
                PERFSTATS_EXIT();
            }
        }
    }
}
