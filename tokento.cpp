#include "common/macros.h"
#include "gameroot.h"
#include "logic/gamelogic/core/tokento.h"
#include "utils/tarslog.h"
#include "context/context.h"
#include "config/gameconfig.h"
#include "process/process.h"
#include "message/sendclientmessage.h"
#include "message/sendroommessage.h"
#include "logic/gamelogic/core/begintimer.h"
#include "logic/gamelogic/core/endtimer.h"
#include "common/nndef.h"
#include "xtime4lib.h"
#include "logic/timeoutlogic/core/bettimeout.h"
#include "third.pb.h"
#include "suoha.pb.h"

using namespace nndef;

namespace game
{
    namespace logic
    {
        namespace gamelogic
        {
            void TokenTo(GameRoot *root)
            {
                PERFSTATS_ENTRY();
                __TRY__

                DLOG_TRACE("roomid:" << root->roomid() << ", " << "TokenTo roomid:" << root->roomid());

                using namespace context;
                using namespace process;
                using namespace message;
                using namespace config;
                using namespace RoomSo;

                //check round end
                if(root->con->checkRoundBetEnd(root->pro->getProcess()))
                {
                    DLOG_TRACE("roomid:" << root->roomid() <<", token to this round is end, process: "<< root->pro->getProcess()); 
                    root->pro->nextProcess();
                    return;
                }

                cid_t tokencid = root->con->getTokenCid();
                User *user = root->con->getUserByCid(tokencid);
                if(!user)
                {
                    DLOG_TRACE("roomid:" << root->roomid() <<"token to err process: "<< root->pro->getProcess() <<" , tokencid: "<< tokencid ); 
                    return;
                }

                E_NN_ACT act = NN_ACT_FOLD;
                act = static_cast<E_NN_ACT>(act | NN_ACT_ALLIN);

                long round_bet_num = user->getRoundBetNum(root->pro->getProcess());
                long call_num = root->con->getRoundMaxBetNum() - round_bet_num;
                if(call_num == 0)
                {
                    act = static_cast<E_NN_ACT>(act | NN_ACT_PASS);
                }
                else if(user->getSHWealth() > call_num)
                {
                    act = static_cast<E_NN_ACT>(act | NN_ACT_FOLLOW);
                }

                long raise_num = call_num + root->cfg->getFrontBet();
                if(user->getSHWealth() > raise_num)
                {
                    act = static_cast<E_NN_ACT>(act | NN_ACT_RAISE);
                }
                
                user->setAct(act);

                XGameSHProto::SH_msg2sTokenNotify shcm;
                shcm.set_icid(tokencid);
                shcm.set_iact(act);
                shcm.set_lcallscore(root->con->getRoundMaxBetNum() - round_bet_num);
                shcm.set_icfgoptiontime(root->cfg->getBetTime());
                shcm.set_iremainoptiontime(0);
               
                sendAllClientMessage<XGameSHProto::SH_msg2sTokenNotify>(XGameSHProto::SH_msg2sTokenNotify_E, shcm, root);
                DLOG_TRACE("roomid:" << root->roomid() <<", token to process: "<< root->pro->getProcess() <<"uid: "<< user->getUid()<<" , tokencid: "<< tokencid << ", shcm:"<< logPb(shcm));
                
                root->con->setTokenOpTime();

                //转发ai 消息
                auto first_robotid = root->con->getRobotUid();
                if(first_robotid != nil_uid && user->isRobot() && root->pro->getProcess() >= NN_STATE_FIRST_CARD && root->pro->getProcess() <= NN_STATE_FOUR_CARD)
                {
                    Pb::ThirdRobotActionReq thirdNotify;
                    thirdNotify.set_roomid("1001");
                    thirdNotify.set_uid(user->getUid());
                    thirdNotify.set_rungameid(first_robotid);
                    thirdNotify.set_street(root->pro->getProcess() - 1);
                    thirdNotify.set_self_allow_actions(act);
                    thirdNotify.set_is_dealer(user->getCid() == root->con->getBankerCid());
                    thirdNotify.set_small_blind(root->cfg->getFrontBet());
                    thirdNotify.set_big_blind(root->cfg->getFrontBet());
                    
                    std::map<int, std::vector<int>> action_list;
                    std::map<cid_t, User> &usermap = root->con->refUserMap();
                    for (auto it = usermap.begin(); it != usermap.end(); it++)
                    {
                        if(it->second.getUid() == user->getUid())
                        {
                            for(auto hdcard : user->getVecCards())
                            {
                                thirdNotify.add_hole_card(hdcard);
                            }
                            thirdNotify.set_self_bet_to(user->getRoundBetNum(-1));
                            thirdNotify.set_self_init_chips(user->getInitWealth());
                            thirdNotify.set_self_street_bet_to(user->getRoundBetNum(root->pro->getProcess()));
                        }
                        else
                        {
                            if(it->second.getVecCards().size() >= 2)
                            {
                                vecc_t lCards;
                                lCards.insert(lCards.begin(), it->second.getVecCards().begin() + 1, it->second.getVecCards().end());
                                
                                for(auto hdcard : lCards)
                                {
                                    thirdNotify.add_board_card(hdcard);
                                } 
                            }
                            
                            thirdNotify.set_oppo_bet_to(it->second.getRoundBetNum(-1));
                            thirdNotify.set_oppo_init_chips(it->second.getInitWealth());
                            thirdNotify.set_oppo_street_bet_to(it->second.getRoundBetNum(root->pro->getProcess()));
                        }

                        for(auto user_action : it->second.getRoundActionList())
                        {
                            auto it = action_list.find(user_action.first);
                            if(it != action_list.end())
                            {
                                it->second.insert(it->second.begin(), user_action.second.begin(), user_action.second.end());
                            }
                            else
                            {
                                std::vector<int> vsub;
                                vsub.insert(vsub.begin(), user_action.second.begin(), user_action.second.end());
                                action_list.insert(std::make_pair(user_action.first, vsub));
                            }
                        }
                        
                    }
                    
                    for(auto action_item : action_list)
                    {
                        Pb::ThirdRobotActionReq::ActionList item;
                        for(auto act : action_item.second)
                        {
                            item.add_action(act);
                        }
                        (*thirdNotify.mutable_action_list())[action_item.first - 1] = item;
                    }

                    DLOG_TRACE("roomid:" << root->roomid()<< ", FrontBet: " << root->cfg->getFrontBet() <<", thirdNotify: "<< logPb(thirdNotify));

                    std::string smsg;
                    thirdNotify.SerializeToString(&smsg);
                    TSendAiData data;
                    data.uid = first_robotid;
                    data.type = Pb::THIRD_CMD_ROBOT_ACTION_REQ;
                    data.msg = smsg;
                    sendRoomMessage<TSendAiData>(TGAME_SendAIData_E, data, root);

                }

                int betTime =  root->cfg->getBetTime();
                if(user->isRobot())
                {
                    betTime = rand() % 5 + 3;
                }
                
                EndTimer(NN_XTIME_GAME_BEGIN, root, false);
                BeginTimer(NN_XTIME_GAME_BEGIN, betTime , [](TimerParam & param)->int
                {
                    auto body = static_cast<std::tuple<GameRoot *> const *>(param.getBody());
                    auto root = std::get<0>(*body);

                    game::logic::timeoutlogic::BetTimeOut(root);
                    return 0;
                }, root, false);
                
                __CATCH__
                PERFSTATS_EXIT();
            }
        }
    }
}
