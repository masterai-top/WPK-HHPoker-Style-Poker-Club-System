# WPK / HHPoker 风格德州扑克俱乐部源码

[简体中文](README.md) | [繁體中文](README.zh-TW.md) | [English](README.en.md) | [在线介绍](https://masterai-top.github.io/WPK-HHPoker-Style-Poker-Club-System/zh-cn/)

> 面向德州俱乐部、私人局、好友局、联赛与 SNG 场景的产品资料与源码样例。仓库包含真实产品截图、Unity 项目资源、C++ 游戏流程代码、Tars 通信接口及项目配置文档。

[![Unity](https://img.shields.io/badge/Client-Unity-222222)](Packages/manifest.json)
[![C++](https://img.shields.io/badge/Server-C%2B%2B-00599C)](gamebegin.cpp)
[![Tars](https://img.shields.io/badge/RPC-Tars-18A999)](JFGame.tars)
[![Contact](https://img.shields.io/badge/Telegram-%40xuzongbin001-229ED9)](https://t.me/xuzongbin001)

## 项目是什么

这是一个 WPK / HHPoker 同类俱乐部产品形态的德州扑克项目资料仓库，重点展示移动端大厅、好友、联赛、SNG、排行榜、用户成长与奖励界面，以及房间和游戏服务之间的接口设计。仓库与 WPK、HHPoker 官方没有隶属、授权或兼容性承诺。

适合用于评估以下方向：

- 德州扑克俱乐部源码、德州私人局与好友局产品设计
- 联赛等级、排名奖励与 SNG 赛事入口
- Unity 客户端资源管理与项目配置
- C++ 房间游戏插件接口、计时器、开局与结算流程
- Tars 客户端、路由、房间和游戏服务消息协议

## 产品功能

| 模块 | 已展示的功能 |
| --- | --- |
| 登录与账号 | 手机、邮箱及游客入口，登录与账号切换界面 |
| 游戏大厅 | 快速游戏、单桌锦标赛、多桌锦标赛、俱乐部入口 |
| 好友社交 | 好友列表、添加/审核、赠礼与聊天入口 |
| 联赛系统 | 段位成长、联赛排名和对应奖励展示 |
| SNG | 不同报名级别、人数与奖励信息 |
| 用户成长 | 等级、成就、牌局数据与个人资料 |
| 运营活动 | 排行榜、宝箱、福利转盘、邮件与奖励展示 |
| 设置 | 音效、震动、语言和账号设置 |

## 玩法流程

1. 玩家从登录页进入大厅，选择快速游戏、SNG、MTT 或俱乐部入口。
2. 好友与私人牌局围绕邀请、加入、桌内互动及战绩关系组织社交体验。
3. SNG 页面按报名条件与奖励展示赛事选择；联赛页面通过段位和排名形成长期目标。
4. 牌桌服务负责玩家消息、旁观消息、房间通信、开局计时与结算流程。

仓库 `Doc/游戏玩法/` 还提供快速游戏、Private 与 SNG 的时序图，可用于理解客户端、业务服务、房间和游戏模块之间的调用关系。

## 真实产品截图

| 游戏大厅 | 好友系统 | SNG 赛事 |
| --- | --- | --- |
| ![德州扑克大厅：快速游戏、SNG、MTT 与俱乐部入口](docs/assets/images/lobby.jpg) | ![德州好友局：好友列表、赠礼与聊天入口](docs/assets/images/friends.jpg) | ![德州 SNG：报名级别与奖励](docs/assets/images/sng.jpg) |

| 联赛段位 | 联赛排名 | 用户资料 |
| --- | --- | --- |
| ![德州扑克联赛段位](docs/assets/images/league-tiers.jpg) | ![德州扑克联赛排名与奖励](docs/assets/images/league-ranking.jpg) | ![玩家等级、成就与数据](docs/assets/images/profile.jpg) |

| 排行榜 | 奖励宝箱 | 福利转盘 |
| --- | --- | --- |
| ![德州扑克排行榜](docs/assets/images/ranking.jpg) | ![游戏奖励宝箱](docs/assets/images/reward-chest.jpg) | ![游戏福利转盘](docs/assets/images/reward-wheel.jpg) |

## 技术结构

```text
Unity 客户端 / 项目资源
        |
        v
Tars 消息包与大厅路由
        |
        v
Room / Table 接口 <-> C++ 游戏动态模块
        |
        +-> 开局、计时、消息广播、旁观与结算样例
        +-> 道具配置、跑马灯配置与运行日志
```

代码与文档中的可验证内容：

- `ITableGame.h`：Game/Table 双向接口、全桌与旁观消息、房间数据交互。
- `JFGameCommProto.tars`：登录大厅、心跳、聊天、语音、GPS、俱乐部房间变化和金币变化消息。
- `gamebegin.cpp`、`gamecalculate.cpp`、`begintimer.cpp`、`endtimer.cpp`：开局、计时和结算相关 C++ 样例。
- `props_config_*.h`、`marquee/`：道具与跑马灯配置管理代码。
- `Packages/`：Unity Package Manager 清单和锁定文件。
- `Doc/`：资源命名、Git 协作、项目配置、热更新、AB 打包与玩法时序资料。

## 仓库范围说明

仓库可直接证明的是上述源码片段、接口、Unity 包配置、文档与截图。是否包含可独立上线所需的全部客户端资源、数据库、后台、第三方服务配置及生产环境依赖，请在获取或部署前逐项核对。README 不把未在仓库中验证的 H5、支付、代理后台或完整商业部署能力写成既定事实。

## 联系与演示

- Telegram：[**@xuzongbin001**](https://t.me/xuzongbin001)
- Email：[**masterai918@gmail.com**](mailto:masterai918@gmail.com)

可联系获取版本范围、完整性说明、演示方式与部署条件。请遵守所在地法律法规及应用商店、支付平台和游戏运营政策。

