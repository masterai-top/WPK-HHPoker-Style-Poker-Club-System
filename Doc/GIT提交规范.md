# GIT提交规范

### Commit message基本格式模板

每次提交，Commit message 都包括三个部分：header，body 和 footer。

```
<type>(<scope>): <subject>  --- Header
<BLANK LINE>
<body>						--- Body
<BLANK LINE>
<footer>					--- Footer
```

其中，header 是必需的，body 和 footer 可以省略。

#### Header

Header分为三部分，首先***type***必需，其次***scope***可选，最后***subject***必需。

##### type（必需）

用于说明 commit 的类别，只允许使用下面标识。

| 类型     | 说明                                                         |
| -------- | ------------------------------------------------------------ |
| feat     | 新增一个新特性或新功能（本身APP/游戏中没有的功能特性）       |
| fix      | 修复一个BUG                                                  |
| docs     | 修改文档，注释修改                                           |
| style    | 格式（不影响代码运行的变动，如空格、空行、代码格式化等相关变动） |
| refactor | 重购（只有在不改变原有功能情况下的代码/界面变更）            |
| test     | 新增/修改单元测试                                            |
| tool     | 开发工具变动(构建、脚手架工具等)                             |
| revert   | 代码回退                                                     |
| opt      | 优化性能、优化功能和优化游戏体验等相关（已有功能更改或扩展都算作优化） |
| chore    | 如配置文件、项目配置等配置相关                               |
| plugin   | 第三方插件新增、删除或升级                                   |

##### scope

**scope**用于描述修改的影响范围。如游戏大厅模块、游戏玩法，签到等相关功能模块。

> 注：若超过2个以上时可以使用**“*”**进行标识。
>
> 注：定义在项目根路径的**.cz-config.js**中。

##### subject（必需）

**subject**主要用于概述本次提交的内容。

### Body

**Body**部分是对本次提交的详细描述，可分为多行描述。如下

```
- 加入好友邀请功能
- 增加世界聊天功能
```

### Footer

**Footer**分两块内容：

一、不兼容变动

如：**接口参数删除，接口移除**等无法向下兼容情况。
则Footer部分使用**BREAKING CHANGE**开头，后面是对变动的描述、以及变动理由和迁移方法。

```
BREAKING CHANGE: 变动描述
	变动理由
	迁移方式
```

二、关闭Issue

Fix #<BUG系统上的ID>

```
Fix #1232
```

# 提交规范化工具

1. 首先安装NVM

> https://github.com/coreybutler/nvm-windows/releases
>
> 添加NVM到环境变量

2. 安装NPM

> nvm install 10.16.3
>
> nvm use 10.16.3

3. 安装cz

> npm install -g commitizen

3. 初始项目

> cd 项目目录
>
> git pull
>
> // 如果下载慢可以尝试改为国内源，参考https://www.jianshu.com/p/0deb70e6f395
>
> npm install

4. 尝试执行git cz进行格式化提交

# 提交操作流程

```flow
st=>start: Start
op1=>operation: 使用git add或第三方工具add, 暂存变更内容
op2=>operation: 执行git cz进行提交
op3=>operation: 确认提交内容
cond1=>condition: Yes or No?
op4=>operation: git reset <commit ID> 取消提交
e=>end: git push

st->op1->op2->op3->cond1->e
cond1(yes)->e
cond1(no)->op4->op1
```

