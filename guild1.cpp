#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <cstdint>

using namespace std;

// ====================== 1. 静态配置表结构体（策划数据表格） ======================
// 军团等级-贡献规则配置表
struct LegionContribCfg
{
    int level;              // 军团等级
    uint32_t dailyMax;     // 每日贡献上限
    uint32_t coinRate;     // 贡献兑换银币比例
};
// 全局静态配表容器，服务启动后只读
unordered_map<int, LegionContribCfg> g_LegionCfgTable;

// ====================== 2. 动态玩家贡献数据（数据库加载的运行时数据） ======================
struct MemberContrib
{
    uint64_t roleId;        // 角色唯一ID
    string roleName;        // 角色昵称
    uint32_t legionId;      // 所属军团ID
    uint32_t totalContrib;  // 历史总贡献
    uint32_t dailyContrib;  // 今日贡献
};

// ====================== 3. 静态表格CSV读取（输入：加载策划配置表） ======================
bool LoadLegionConfigTable(const string& csvPath)
{
    ifstream fin(csvPath);
    if (!fin.is_open())
    {
        cerr << "[错误] 无法打开配置表文件：" << csvPath << endl;
        return false;
    }

    string line;
    // 跳过表头行
    getline(fin, line);

    while (getline(fin, line))
    {
        stringstream ss(line);
        string field;
        LegionContribCfg cfg;

        getline(ss, field, ','); cfg.level = stoi(field);
        getline(ss, field, ','); cfg.dailyMax = stoul(field);
        getline(ss, field, ','); cfg.coinRate = stoul(field);

        g_LegionCfgTable[cfg.level] = cfg;
    }

    fin.close();
    cout << "[加载成功] 军团规则配置表，共" << g_LegionCfgTable.size() << "条\n" << endl;
    return true;
}

// 查询静态配置表（表格输出查询结果）
void ShowLegionRule(int lv)
{
    auto it = g_LegionCfgTable.find(lv);
    if (it == g_LegionCfgTable.end())
    {
        cout << "未找到等级" << lv << "的军团配置\n";
        return;
    }
    auto& cfg = it->second;
    cout << "===== 军团等级" << lv << "规则 =====" << endl;
    cout << "每日贡献上限：" << cfg.dailyMax << endl;
    cout << "贡献兑银币比例：1贡献=" << cfg.coinRate << "银币\n" << endl;
}

// ====================== 4. 贡献排序核心比较函数 ======================
// 排序规则：总贡献降序；总贡献相等，今日贡献降序
bool CmpContribRank(const MemberContrib& a, const MemberContrib& b)
{
    if (a.totalContrib != b.totalContrib)
    {
        return a.totalContrib > b.totalContrib;
    }
    return a.dailyContrib > b.dailyContrib;
}

// 筛选指定军团 + 贡献排序
vector<MemberContrib> GetLegionRankList(const vector<MemberContrib>& allData, uint32_t legionId)
{
    vector<MemberContrib> res;
    // 过滤目标军团成员
    for (const auto& data : allData)
    {
        if (data.legionId == legionId)
        {
            res.push_back(data);
        }
    }
    // 执行排序
    sort(res.begin(), res.end(), CmpContribRank);
    return res;
}

// ====================== 5. 输出功能1：控制台打印排行榜 ======================
void PrintRankToConsole(const vector<MemberContrib>& rankList)
{
    cout << "==================== 军团贡献排行榜 ====================" << endl;
    cout << "排名\t角色ID\t\t昵称\t总贡献\t今日贡献" << endl;
    for (size_t i = 0; i < rankList.size(); ++i)
    {
        const auto& m = rankList[i];
        cout << i + 1 << "\t"
             << m.roleId << "\t"
             << m.roleName << "\t"
             << m.totalContrib << "\t"
             << m.dailyContrib << endl;
    }
    cout << "========================================================\n" << endl;
}

// ====================== 6. 输出功能2：导出排序结果到CSV数据表 ======================
bool ExportRankToCsv(const vector<MemberContrib>& rankList, const string& outPath)
{
    ofstream fout(outPath);
    if (!fout.is_open())
    {
        cerr << "[错误] 无法写入排行文件：" << outPath << endl;
        return false;
    }
    // 写入表头
    fout << "rank,roleId,roleName,totalContrib,dailyContrib" << endl;
    // 逐行写入排行数据
    for (size_t i = 0; i < rankList.size(); ++i)
    {
        const auto& m = rankList[i];
        fout << i + 1 << ","
             << m.roleId << ","
             << m.roleName << ","
             << m.totalContrib << ","
             << m.dailyContrib << endl;
    }
    fout.close();
    cout << "[导出成功] 排行榜已保存至：" << outPath << "\n" << endl;
    return true;
}

// ====================== 主函数：完整流程演示 ======================
int main()
{
    // ---------- 步骤1：输入静态配置表格（CSV策划表） ----------
    // 先新建 legion_contrib_cfg.csv 文件，内容见代码下方说明
    if (!LoadLegionConfigTable("legion_contrib_cfg.csv"))
    {
        return -1;
    }
    // 查询3级军团规则
    ShowLegionRule(3);

    // ---------- 步骤2：模拟从数据库读取全服玩家贡献动态数据 ----------
    vector<MemberContrib> allPlayerData = {
        {10001, "战神一号", 1001, 5200, 300},
        {10002, "默默挂机", 1001, 6800, 200},
        {10003, "独行侠客", 1002, 9000, 500},
        {10004, "氪金大佬", 1001, 6800, 450},
        {10005, "咸鱼玩家", 1001, 3100, 100},
        {10006, "军团长老", 1002, 7500, 600},
    };

    // ---------- 步骤3：筛选军团1001成员并排序 ----------
    uint32_t targetLegion = 1001;
    vector<MemberContrib> rankResult = GetLegionRankList(allPlayerData, targetLegion);

    // ---------- 步骤4：输出1 控制台打印排行 ----------
    PrintRankToConsole(rankResult);

    // ---------- 步骤5：输出2 导出排序表格CSV ----------
    ExportRankToCsv(rankResult, "legion_1001_rank.csv");

    cout << "程序执行完毕！" << endl;
    return 0;
}
