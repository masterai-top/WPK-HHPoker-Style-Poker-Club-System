#include "Comm/ITableGame.h"
#include "common/macros.h"
#include "common/nndef.h"
#include "common/nnlogic.h"
#include "gameroot.h"
#include "logic/gamelogic/core/begintimer.h"
#include "logic/gamelogic/core/endtimer.h"
#include "logic/gamelogic/core/gamebegin.h"
#include "utils/tarslog.h"
#include "context/context.h"
#include "suoha.pb.h"
#include "process/process.h"
#include "message/sendclientmessage.h"
#include "message/sendroommessage.h"
#include "config/gameconfig.h"
#include "common/nndef.h"
#include "xtime4lib.h"
#include "CommonCode.pb.h"
#include "third.pb.h"

using namespace nndef;

namespace game
{
    namespace logic
    {
        namespace gamelogic
        {
            void GameBegin(GameRoot *root)
            {
                PERFSTATS_ENTRY();
                __TRY__

                DLOG_TRACE("roomid:" << root->roomid() << ", " << "GameBegin roomid:" << root->roomid());

                using namespace context;
                using namespace process;
                using namespace message;
                using namespace config;
                using namespace RoomSo;
                using namespace gamelogic;

                std::vector<long> vUserList;
                std::map<cid_t, User> &usermap = root->con->refUserMap();
                if(usermap.size() >= 2) //满足开局
                {
                    XGameSHProto::SH_msg2cGameBegin shcm;

                    //step1 定庄 
                    cid_t bankercid = root->con->setBankerCid(root->cfg->getMaxSeatNum());
                    root->con->setTokenCid(bankercid);

                    //step 2 前注
                    int minStartWealth = root->cfg->getFrontBet();
                    for (auto it = usermap.begin(); it != usermap.end(); it++)
                    {
                        if (it->second.isMidSit())
                        {
                            continue;
                        }
                        if(it->second.getSHWealth() < minStartWealth)
                        {
                            minStartWealth = it->second.getSHWealth();
                        }
                    }

                    for (auto it = usermap.begin(); it != usermap.end(); it++)
                    {
                        if (it->second.isMidSit())
                        {
                            continue;
                        }

                        (*shcm.mutable_mbasescore())[it->first] = minStartWealth;

                        it->second.setInitWealth(it->second.getSHWealth());
                        it->second.setSHWealth(it->second.getSHWealth() - minStartWealth);
                        it->second.addRoundBetnumList(root->pro->getProcess(), minStartWealth);
                        it->second.addRoundActionList(root->pro->getProcess(), 1024);
                        root->con->addTotalPoolNum(minStartWealth);

                        (*shcm.mutable_mcidscore())[it->first] = it->second.getSHWealth();

                        vUserList.push_back(it->second.getUid());
                        DLOG_TRACE("roomid:" << root->roomid() << ", gamebegin cid" << it->first << ", uid: "<< it->second.getUid() 
                            << ", shwealth: "<< it->second.getSHWealth() <<", round bet: "<< it->second.getRoundBetNum(root->pro->getProcess() + 1));
                    
                        //牌局详情
                        auto& gameDetails = root->con->getGameDetails();
                        gameDetails.set_ltotalpool(root->con->getTotalPoolNum());
                        auto stepinfo = gameDetails.add_stepinfo();
                        stepinfo->set_ichairid(it->first);
                        stepinfo->set_iaction(1024);
                        stepinfo->set_lbetnum(minStartWealth);
                        stepinfo->set_lwealth(it->second.getSHWealth());
                        stepinfo->set_iround(root->pro->getProcess() - 1);
                        stepinfo->set_betraio((minStartWealth * 1.00) / root->con->getTotalPoolNum());
                    }
                    shcm.set_ibankercid(bankercid);
                    shcm.set_lpoolscore(0);

                    DLOG_TRACE("roomid:" << root->roomid() << ", gamebegin shcm:" << logPb(shcm));
                    sendAllClientMessage<XGameSHProto::SH_msg2cGameBegin>(XGameSHProto::SH_msg2cGameBegin_E, shcm, root);

                    for (auto it = usermap.begin(); it != usermap.end(); it++)
                    {
                        if (it->second.isMidSit())
                        {
                            continue;
                        }
                        if(it->second.getSHWealth() <= 0)
                        {
                            it->second.setOption(NN_ACT_ALLIN);
                            it->second.setAllIn(true);
                            root->con->setFirstEnd(true);
                        }
                        it->second.setDone(true);
                    }

                    //推送ai消息
                    auto first_robotid = root->con->getRobotUid();
                    if(first_robotid != nil_uid)
                    {
                        Pb::ThirdGameBeginNotify thirdNotify;
                        thirdNotify.set_roomid("1001");
                        for (auto it = usermap.begin(); it != usermap.end(); it++)
                        {
                            User *user = root->con->getUserByCid(it->first);
                            if(!user)
                            {
                                continue;
                            }
                            auto third_seat  = thirdNotify.add_seats();
                            third_seat->set_uid(user->getUid());
                            third_seat->set_chip(user->getSHWealth());
                            third_seat->set_seatno(user->getCid());
                            third_seat->set_isrobot(user->isRobot()? 1: 0);
                            if(!user->isRobot())
                            {
                                thirdNotify.set_player_id(user->getUid());
                                thirdNotify.set_player_score(user->getSHWealth());
                            }
                        }

                        std::string smsg;
                        thirdNotify.SerializeToString(&smsg);
                        TSendAiData data;
                        data.uid = first_robotid;
                        data.type = Pb::THIRD_CMD_GAME_BEGIN_NOTIFY;
                        data.msg = smsg;
                        sendRoomMessage<TSendAiData>(TGAME_SendAIData_E, data, root);

                    }

                    //推送房间游戏开始
                    TGAME_GameBegin tmBegin;
                    tmBegin.vecUList = vUserList;
                    sendRoomMessage<TGAME_GameBegin>(TGAME_GameBegin_E, tmBegin, root);

                    root->con->setRoundMaxBetNum(root->cfg->getFrontBet());

                    root->con->setBeginTime(time(nullptr));

                    vecc_t &vecWallCards = root->con->refVecWallCard();
                    //筑牌
                    nnlogic::build(vecWallCards);
                    //洗牌
                    nnlogic::shuffle<card_t>(vecWallCards);

                    DLOG_TRACE("roomid:" << root->roomid() << ", gamebegin time: " << root->cfg->getDelayTime());
                    EndTimer(NN_XTIME_GAME_BEGIN, root, false);
                    BeginTimer(NN_XTIME_GAME_BEGIN, root->cfg->getDelayTime() * 2, [](TimerParam & param)->int
                    {
                        auto body = static_cast<std::tuple<GameRoot *> const *>(param.getBody());
                        auto root = std::get<0>(*body);
                        DLOG_TRACE("roomid:" << root->roomid() << ", gamebegin next.");

                        root->pro->nextProcess();

                        return 0;
                    }, root, false);
                }

                __CATCH__
                PERFSTATS_EXIT();
            }
        }
    }
}
