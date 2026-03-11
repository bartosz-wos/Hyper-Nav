#include "../include/SuffixAutomaton.h"

std::vector<State> st;
long long sz;
long long last;
std::vector<std::vector<long long>> inv_link;

void sa_init(){
        st.clear();
        st.push_back({0, -1, -1, {}});
        sz = 1;
        last = 0;
}

void sa_extend(char c, long long current_pos){
        long long cur = sz++;
        st.emplace_back();

        st[cur].len = st[last].len + 1;
        st[cur].first_pos = current_pos;
        st[cur].next.clear();
        st[cur].link = -1;

        long long p = last;
        while(p != -1 && st[p].next.find(c) == st[p].next.end()){
                st[p].next[c] = cur;
                p = st[p].link;
        }
        if(p == -1){
                st[cur].link = 0;
        }else{
                long long q = st[p].next[c];
                if(st[p].len + 1 == st[q].len){
                        st[cur].link = q;
                }else{
                        long long clone = sz++;
                        st.emplace_back();
                        st[clone].len = st[p].len + 1;
                        st[clone].next = st[q].next;
                        st[clone].link = st[q].link;
                        st[clone].first_pos = st[q].first_pos;
                        while(p != -1 && st[p].next[c] == q){
                                st[p].next[c] = clone;
                                p = st[p].link;
                        }
                        st[q].link = st[cur].link = clone;
                }
        }
        last = cur;
}

void build_tree(){
        inv_link.assign(sz, std::vector<long long>());
        for(long long node = 1; node < sz; node++)
                inv_link[st[node].link].push_back(node);
}

void get_occurences(long long v, std::vector<long long>& positions){
        if(st[v].first_pos != -1){
                positions.push_back(st[v].first_pos);
        }
        for(const long long&u : inv_link[v]){
                get_occurences(u, positions);
        }
}
