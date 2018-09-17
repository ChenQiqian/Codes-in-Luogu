// luogu-judger-enable-o2
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;

const int MAXN = 110000;

int n,m,k,Q;
int num[MAXN];
int xor1[MAXN];
int xor2[MAXN];
int ans[MAXN];

struct Query{
    int l,r,id;
    bool operator <(const Query _q)const{
        if(l/Q != _q.l/Q){
            return l/Q < _q.l/Q;
        }
        else{
            return r < _q.r;
        }
    }
}q[MAXN];

void init(){
    scanf("%d %d %d",&n,&m,&k);
    Q = sqrt(n);
    for(int i = 1;i<=n;i++){
        scanf("%d",&num[i]);
    }
    for(int i = 1;i<=m;i++){
        scanf("%d %d",&q[i].l,&q[i].r);
        q[i].id = i;
    }
    sort(q+1,q+m+1);
}

void build(){
    for(int i = 1;i<=n;i++){
        xor1[i] = xor1[i-1] ^ num[i];
        xor2[i] = xor2[i-1] ^ num[i];
    }
    for(int i = 0;i<=n;i++){
        xor2[i] ^= k;
    }
    for(int i = n;i>=0;i--){
        xor2[i+1] = xor2[i];
    }
//	for(int i = 1;i<=n;i++){
//		printf("%d: xor1:%d xor2:%d\n",i,xor1[i],xor2[i]);
//	}
}

void solve(){
    for(int i = 1;i<=m;i++){
        int L = q[i].l,R = q[i].r;
        int tmp = 0;
        for(int a = L;a<=R;a++){
            for(int b = L;b<=a;b++){
                if(xor1[a] == xor2[b]){
                    tmp++;
                }
            }
        }
        ans[q[i].id] = tmp;
    }
    for(int i = 1;i<=m;i++){
        printf("%d\n",ans[i]);
    }
}

int main(){
    init();
    build();
    solve();
    return 0;
}