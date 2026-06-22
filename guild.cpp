#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Player {
    int guildId;
    int guildLevel;
    int playerId;
    int contribution;
};

struct RewardRule {
    int topPercent;
    string reward;
};

struct Result {
    int guildId;
    int playerId;
    int rank;
    double percent;
    string reward;
};

int main() {

    int m;
    cin >> m;

    unordered_map<int, vector<RewardRule>> config;

    for (int i = 0; i < m; i++) {
        int guildLevel;
        int topPercent;
        string reward;

        cin >> guildLevel >> topPercent >> reward;

        config[guildLevel].push_back({
            topPercent,
            reward
        });
    }

    for (auto &kv : config) {
        sort(
            kv.second.begin(),
            kv.second.end(),
            [](const RewardRule &a,
               const RewardRule &b) {
                return a.topPercent < b.topPercent;
            }
        );
    }

    int n;
    cin >> n;

    unordered_map<int, vector<Player>> guilds;

    for (int i = 0; i < n; i++) {

        Player p;

        cin >> p.guildId
            >> p.guildLevel
            >> p.playerId
            >> p.contribution;

        guilds[p.guildId].push_back(p);
    }

    vector<Result> ans;

    for (auto &kv : guilds) {

        auto &players = kv.second;

        sort(
            players.begin(),
            players.end(),
            [](const Player &a,
               const Player &b) {

                if (a.contribution != b.contribution)
                    return a.contribution > b.contribution;

                return a.playerId < b.playerId;
            }
        );

        int total = players.size();

        for (int i = 0; i < total; i++) {

            int rank = i + 1;

            double percent =
                rank * 100.0 / total;

            int level =
                players[i].guildLevel;

            string reward = "None";

            auto &rules = config[level];

            for (auto &rule : rules) {

                if (percent <= rule.topPercent) {
                    reward = rule.reward;
                    break;
                }
            }

            ans.push_back({
                players[i].guildId,
                players[i].playerId,
                rank,
                percent,
                reward
            });
        }
    }

    sort(
        ans.begin(),
        ans.end(),
        [](const Result &a,
           const Result &b) {

            if (a.guildId != b.guildId)
                return a.guildId < b.guildId;

            return a.rank < b.rank;
        }
    );

    cout << "GuildID PlayerID Rank Percent Reward\n";

    for (auto &r : ans) {

        cout << r.guildId << ' '
             << r.playerId << ' '
             << r.rank << ' '
             << fixed << setprecision(2)
             << r.percent << ' '
             << r.reward << '\n';
    }

    return 0;
}