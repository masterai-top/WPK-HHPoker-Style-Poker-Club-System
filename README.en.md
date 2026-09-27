# WPK / HHPoker-Style Poker Club Source Code

[简体中文](README.md) | [繁體中文](README.zh-TW.md) | [English](README.en.md) | [Live overview](https://masterai-top.github.io/WPK-HHPoker-Style-Poker-Club-System/en/)

> Product materials and source samples for a Texas Hold'em club, private-table, friends-game, league and SNG experience. The repository includes real product screenshots, Unity project resources, C++ game-flow code, Tars interfaces and project documentation.

[![Unity](https://img.shields.io/badge/Client-Unity-222222)](Packages/manifest.json)
[![C++](https://img.shields.io/badge/Server-C%2B%2B-00599C)](gamebegin.cpp)
[![Tars](https://img.shields.io/badge/RPC-Tars-18A999)](JFGame.tars)
[![Contact](https://img.shields.io/badge/Telegram-%40xuzongbin001-229ED9)](https://t.me/xuzongbin001)

## Product Positioning

This repository presents a poker-club product in the same general category as WPK/HHPoker: a mobile lobby, friends and private-game journeys, league progression, SNG selection, rankings, player progression and reward surfaces. It also exposes room/game interface examples for technical evaluation. The project is not affiliated with, authorized by, or guaranteed to be compatible with WPK or HHPoker.

It is relevant to teams researching:

- Texas Hold'em poker club source code and private poker rooms
- Friend-game and social lobby product design
- League tiers, rankings and SNG tournament entry
- Unity project/package organization
- C++ room-game plug-in interfaces and Tars service messages

## Product Features

| Area | Features visible in this repository |
| --- | --- |
| Sign-in | Phone, email and guest entry points |
| Lobby | Quick Game, single-table tournament, multi-table tournament and club access |
| Social | Friends list, requests, gifts and chat entry points |
| League | Tier progression, rankings and reward tables |
| SNG | Multiple entry levels, player counts and prize displays |
| Player profile | Level, achievements and game statistics |
| Live operations | Rankings, reward chest, prize wheel and mail screens |
| Settings | Sound, vibration, language and account options |

## Player Journey

1. A player signs in and enters a lobby with Quick Game, SNG, MTT and club entry points.
2. Friends and private-table flows organize invitations, joining and in-table social interaction.
3. SNG cards expose entry requirements and prizes; league tiers and rankings provide long-term progression.
4. Room and table services exchange player, spectator and game messages while C++ samples cover timers, game start and settlement-related flow.

Sequence diagrams for Quick Game, Private and SNG under `Doc/` show the interaction among client, business services, room services and game modules.

## Real Product Screenshots

| Lobby | Friends | SNG |
| --- | --- | --- |
| ![Poker lobby with Quick Game, SNG, MTT and club entry](docs/assets/images/lobby.jpg) | ![Friends, gifts and chat entry points](docs/assets/images/friends.jpg) | ![SNG entries and prizes](docs/assets/images/sng.jpg) |

| League tiers | League ranking | Player profile |
| --- | --- | --- |
| ![Poker league tier progression](docs/assets/images/league-tiers.jpg) | ![League ranking and rewards](docs/assets/images/league-ranking.jpg) | ![Player level, achievements and statistics](docs/assets/images/profile.jpg) |

| Ranking | Reward chest | Prize wheel |
| --- | --- | --- |
| ![Poker ranking screen](docs/assets/images/ranking.jpg) | ![Reward chest](docs/assets/images/reward-chest.jpg) | ![Prize wheel](docs/assets/images/reward-wheel.jpg) |

## Technical Structure

```text
Unity client / project resources
        -> Tars messages and lobby routing
        -> Room / Table API <-> C++ game module
        -> start, timer, broadcast, spectator and settlement samples
```

- `ITableGame.h` defines bidirectional Game/Table APIs, room calls, broadcasts and spectator messages.
- `JFGameCommProto.tars` defines lobby login, keep-alive, text/voice chat, GPS and club/gold update messages.
- `gamebegin.cpp`, `gamecalculate.cpp`, `begintimer.cpp` and `endtimer.cpp` provide C++ flow samples.
- `props_config_*.h` and `marquee/` contain configuration-management code.
- `Packages/` holds Unity package manifests; `Doc/` covers project configuration, hot updates, asset bundles, conventions and gameplay sequences.

## Repository Scope

The code fragments, interfaces, Unity package files, documents and screenshots above are directly verifiable. Before deployment, verify whether the version you obtain includes every client asset, database, administration service, third-party integration and production dependency you need. This README does not present unverified H5, payment, agency-console or turnkey production capabilities as confirmed features.

## Contact and Demo

- Telegram: [**@xuzongbin001**](https://t.me/xuzongbin001)
- Email: [**masterai918@gmail.com**](mailto:masterai918@gmail.com)

Contact us for version scope, completeness, demonstration options and deployment requirements. Follow applicable laws and app-store, payment and gaming-platform policies.

