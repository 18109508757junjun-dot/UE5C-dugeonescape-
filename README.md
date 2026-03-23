# 📁 组件化地牢交互系统 (Component-Based Dungeon Interaction)

[![Unreal Engine](https://img.shields.io/badge/Unreal%20Engine-5.7-white?logo=unrealengine&logoColor=white&color=0E1128)](https://www.unrealengine.com/)
[![Language](https://img.shields.io/badge/Language-C++-blue.svg)](https://isocpp.org/)

## 📝 项目简介
本项目是一个基于 **UE5 C++** 开发的高度模块化环境交互框架。核心通过 **Actor Component** 模式实现了触发逻辑与位移表现的深度解耦。该系统支持多种交互方式（球形扫掠检测、压力板重叠触发），并能通过简单的组件挂载与参数配置，快速构建复杂的关卡机关，如机关门、钥匙锁、可收集物等。

> [!IMPORTANT]
> **注意：** 本仓库为 **Code-Only** 模式，包含完整的交互系统 C++ 源码。建议重点审阅 `TriggerComponent` 对 `UMover` 的动态引用以及玩家类的射线交互逻辑。

---

## 🛠 技术栈 (Tech Stack)

| 分类 | 工具/技术 |
| :--- | :--- |
| **引擎版本** | **Unreal Engine 5.7** |
| **核心架构** | **组件化设计 (Component-Based)** / **标签系统 (Tag System)** |
| **交互技术** | **SweepSingleByChannel** (球形扫掠检测) / **Delegates** (动态绑定) |
| **开发环境** | **Visual Studio**, **GitHub Copilot** (AI 辅助逻辑优化) |
| **版本控制** | **Git** |

---

## 🧠 核心模块深度解析

### 1. 深度解耦的组件通信架构
* **动态检索机制：** `TriggerComponent` 并不硬编码依赖特定的 Actor，而是通过 `FindComponentByClass<UMover>()` 在运行时动态检索目标组件。
* **逻辑复用：** 触发器（如压力板、钥匙锁）与受控物体完全分离。只要目标 Actor 挂载了 `UMover` 组件，即可通过指针实现跨 Actor 的逻辑触发。

### 2. 射线与球形扫掠交互系统 (Interact System)
* **核心逻辑：** 在 `DugeonEscapeCharacter.cpp` 中实现了基于摄像机视角的交互检测。
* **技术细节：** 结合 `SweepSingleByChannel` 进行球形扫掠检测，配合 **Actor Tag (标签系统)** 区分 “CollectableItem” 与 “Lock”。
* **背包联动：** 实现了完整的物品收集逻辑（`ItemList`）与机关联动，包括钥匙的消耗、放置以及锁状态的实时更新。

### 3. 高性能平滑位移组件 (Mover Component)
* **核心算法：** 采用 `FMath::VInterpConstantTo` 实现位置平滑插值。
* **性能优化策略：** **针对 Tick 进行深度优化**。将目标位置计算移出 `TickComponent`，仅在 `SetShouldMove` 触发时计算一次，显著降低了常驻每帧逻辑的 CPU 开销。

### 4. 多态触发机制 (Flexible Triggers)
* **压力板模式：** 动态绑定 `OnComponentBeginOverlap`，通过计数器精准处理多个激活物同时重叠的边界情况。
* **手动触发接口：** 通过 `Trigger(bool)` 接口支持从外部代码（如玩家交互脚本）直接调用，实现了单一组件对多种交互情景的兼容。

---

## 📂 如何阅读本项目 (How to Navigate)

由于缺少资产，源码主要集中在 `Source/` 目录下。建议按以下逻辑链条审阅：

* **交互发起端：** `Source/.../DugeonEscapeCharacter.cpp`
    * *观察点：球形扫掠检测逻辑、基于 Tag 的物品识别与背包管理。*
* **逻辑调度中枢：** `Source/.../TriggerComponent.cpp`
    * *观察点：组件动态查找、重叠事件绑定及对 Mover 的指令下发。*
* **物理执行端：** `Source/.../Mover.cpp`
    * *观察点：插值平滑算法、Tick 性能优化技巧。*
* **机关载体实现：** `Source/.../Lock.cpp` 与 `Source/.../CollectableItem.cpp`
    * *观察点：标签初始化与复合组件的 SetupAttachment。*
> **提示：** 源码中关键逻辑均附带详细的中文注释及目录说明。

---

## 📜 许可声明 (License)
本项目仅用于个人学习交流与作品集展示。
* **美术素材：** 来源于网络公开资源（如虚幻商城等），版权归原作者所有。
