# GIT协作规范

### 从团队项目FORK与配置

1. 打开团队项目页面，点击FORK项目到自己的命名空间
2. 将FORK的项目使用 git clone 到本地开发环境中

3. 使用 git remote add upstream <团队项目repo地址>，将本地与团队的repo建立联系。
4. 使用 git pull upstream <分支名称> 将团队项目上的更新拉取到本地中

### 提交与PR操作流程

1. 首先，当在FORK项目中进行开发完成提交并推送到远端FORK项目后，打开FORK项目页面选择**Merge Requests->New merge requests**，选择**源分支**和要合并到的**目标分支**
2. 然后，点击**Compare branches and continue**确认提交内容并填写合并内容描述。
3. 最后，确认内容正确无误后**Submit merge request**.



# *特性分支

***特性分支***是从当前HEAD/指定tag/指定commit创建新一个分支专门用于开发/修复一个特性/功能/BUG。若有多个特性需开发可创建多个分支，然后逐个进行合并到主干分支上，最后发起PR。

#### 从当前HEAD

> git checkout -b <分支名称>

#### 从指定Tag

> git branch <分支名称> <TAG名称>
>
> git checkout <分支名称>

#### 从指定Commit

> git branch <分支名称> <Commint ID>
>
> git checkout <分支名称>

### 操作流程图

```flow
st=>start: Start
op1=>operation: 从团队项目仓库FORK到自己的命名空间
op2=>operation: 将FORK后的项目clone到本地
op3=>operation: 使用git remote add upstream <团队项目repo地址>进行关联
op4=>operation: 切换到团队项目的开发分支（或）从该分支创建特性分支
op6=>operation: 在当前分支中进行功能开发并提交推送变更
op7=>operation: git pull upstream <分支名称>，先同步团队项目目前变更内容
op8=>operation: 将working分支合并到本地的团队开发分支
cond1=>condition: 是否冲突
op9=>operation: 合并冲突并提交
op10=>operation: 推送合并后内容到远端FORK项目
op11=>operation: 到FORK项目页面发起合并请求
e=>end: End
st->op1->op2->op3->op4->op6->op7->op8->cond1->e
cond1(yes)->op9->op10->op11
cond1(no)->op10->op11->e

```

