#include <bits/stdc++.h>
using namespace std;

const int MAXN = 510000;
const int MAX = 2147483647;

namespace fast_io {
    inline char read(){static const int IN_LEN=1000000;static char buf[IN_LEN],*s,*t;return s==t?(((t=(s=buf)+fread(buf,1,IN_LEN,stdin))==s)?-1:*s++) : *s++;}
    inline void read(int &x){static bool iosig;static char c;for (iosig=false,c=read();!isdigit(c);c=read()){if(c=='-')iosig=true;if(c==-1)return;}for(x=0;isdigit(c);c=read())x=((x+(x<<2))<<1)+(c^'0');if(iosig)x=-x;}
    inline void read(char *a){static char c = read();while(c!=-1&&(c==' '||c=='\n'||c=='\r'))c=read();while(c!=-1&&c!='\r'&&c!=' '&&c!='\n') *a++=c,c=read();*a=0;}
    const int OUT_LEN=1000000;char obuf[OUT_LEN],*ooh=obuf;
    inline void print(char c){if(ooh==obuf+OUT_LEN) fwrite(obuf,1,OUT_LEN,stdout),ooh=obuf;*ooh++ = c;}
    inline void print(int x){static int buf[30],cnt;if(x==0)print('0');else{if(x<0)print('-'),x=-x;for(cnt=0;x;x/=10)buf[++cnt]=x%10+48;while (cnt) print((char)buf[cnt--]);}}
    inline void print(char *a){while(*a) print(*a++);}
    inline void flush(){fwrite(obuf,1,ooh-obuf,stdout);}
}using namespace fast_io;

int k,n;

struct node_t{
    int val,p,lmax,rmax,maxn,sumn,tag;
    int size;
    bool rev,is_tag;
    node_t *son[2],**null;
    void pushdown(){
        if(this == *null) return;
        if(is_tag){
            son[0]->cover(tag),son[1]->cover(tag);
            is_tag = tag = 0;
        }
        if(rev) {
            son[0]->reverse(),son[1]->reverse();
            rev = 0;
        }
    }
    void pushup(){
        if(this == *null) return;
        if(son[0] == *null && son[1] == *null){
            size = 1;sumn = lmax = rmax = maxn = val;
            return;
        }
        size = son[0]->size + son[1]->size + 1;
        sumn = son[0]->sumn + son[1]->sumn + val;
        lmax = max(son[0]->lmax,son[0]->sumn + val + max(0,son[1]->lmax));
        rmax = max(son[1]->rmax,son[1]->sumn + val + max(0,son[0]->rmax));
        maxn = max(0,son[0]->rmax) + val + max(0,son[1]->lmax);
        maxn = max(maxn,max(son[0]->maxn,son[1]->maxn));
    }
    void cover(int v){
        if(this == *null) return;
        val = v;sumn = size * v;
        lmax = rmax = maxn = max(v,sumn);
        is_tag = 1;tag = v;
    }
    void reverse(){
        if(this == *null) return;
        swap(son[0],son[1]);
        swap(lmax,rmax);
        rev^=1;
    }
};


struct fhqtreap{
    node_t pool[MAXN],*tmp[MAXN],*stack[MAXN],*garbage[MAXN];
    node_t *root,*null;
    int cnt,tot;
    void newnode(node_t *&r,int val = 0){
        if(tot == 0)
            r = &pool[cnt++];
        else
            r = garbage[--tot];
        r->val = val;r->size = 1;
        r->lmax = r->rmax = r->maxn = r->sumn = val;
        r->son[0] = r->son[1] = null;
        r->is_tag = r->rev = 0;
        r->null = &null;
        r->p = rand();
    }
    fhqtreap(){
        tot = 0;cnt = 0;
        srand(time(NULL));
        newnode(null,-MAX);
        null->p = MAX;
        root = null;
        null -> sumn = null->size = 0;
    }
    void cycle(node_t *r){
        if(r == null) return;
        garbage[tot++] = r;
        cycle(r->son[0]);
        cycle(r->son[1]);
    }
    void read_tree(int n){
        for(int i = 1;i<=n;i++){
            int t;scanf("%d",&t);
            newnode(tmp[i],t);
        }
    }
    node_t *build(int n){
        read_tree(n);
        int top = 1;
        newnode(stack[0],-MAX);
        stack[0]->p = -MAX;
        for(int i = 1;i<=n;i++){
            int nowp = top - 1;
            node_t *r = tmp[i],*pre = null;
            while(stack[nowp]->p > r -> p){
                stack[nowp]->pushup();
                pre = stack[nowp];
                stack[nowp] = null;
                nowp--;
            }
            stack[nowp+1] = stack[nowp]->son[1] = r;
            stack[nowp+1]->son[0] = pre;
            top = nowp+2;
        }
        while(top) stack[--top]->pushup();
        return stack[0]->son[1];
    }
    void split(node_t *r,int lsize,node_t *&ls,node_t *&rs){
        if(r == null){
            ls = null;rs = null;
            return;
        }
        r->pushdown();
        if(r->son[0]->size + 1 <= lsize){
            ls = r;
            split(r->son[1],lsize - r->son[0]->size - 1,ls->son[1],rs);
        }
        else{
            rs = r;
            split(r->son[0],lsize,ls,rs->son[0]);
        }
        ls->pushup();rs->pushup();
    }
    node_t *merge(node_t *ls,node_t *rs){
        if(ls == null) return rs;
        if(rs == null) return ls;
        if(ls->p < rs->p){
            ls->pushdown();
            ls->son[1] = merge(ls->son[1],rs);
            ls->pushup();
            return ls;
        }
        else{
            rs->pushdown();
            rs->son[0] = merge(ls,rs->son[0]);
            rs->pushup();
            return rs;
        }
    }
    void insert(int rank,int n){
        if(n == 0) return;
        node_t *ls,*rs,*newn,*ret;
        split(root,rank,ls,rs);
        newn = build(n);
        root = merge(merge(ls,newn),rs);
    }
    void split(int ls,int ms,node_t *&l,node_t *&m,node_t *&r){
        node_t *m1;
        split(root,ls,l,m1);
        split(m1,ms,m,r);
    }
    void erase(int lb,int ms){
        if(ms == 0) return;
        node_t *l,*m,*r,*ret;
        split(lb-1,ms,l,m,r);
        cycle(m);
        root = merge(l,r);
    }
    int get_sum(int lb,int ms){
        if(ms == 0) return 0;
        node_t *l,*m,*r;
        split(lb-1,ms,l,m,r);
        int ans = m->sumn;
        root = merge(l,merge(m,r));
        return ans;
    }
    int max_sum(){
        return root->maxn;
    }
    void reverse(int lb,int ms){
        if(ms == 0) return;
        node_t *l,*m,*r;
        split(lb-1,ms,l,m,r);
        m->reverse();       
        root = merge(l,merge(m,r));
    }
    void make_same(int lb,int ms,int c){
        if(ms == 0) return;
        node_t *l,*m,*r;
        split(lb-1,ms,l,m,r);
        m->cover(c);
        root = merge(l,merge(m,r));
    }
};

fhqtreap w;

void init(){
    scanf("%d %d",&n,&k);
    w.root = w.build(n);
}

void solve(){
    for(int i = 1;i<=k;i++){
        char op[50];int a,b,c;
        scanf("%s",op);
        if(op[0] == 'M'&&op[2] == 'X'){
            printf("%d\n",w.max_sum());
        }
        else{
            scanf("%d %d",&a,&b);
            if(op[0] == 'I'){
                w.insert(a,b);
            }
            else if(op[0] == 'D'){
                w.erase(a,b);
            }
            else if(op[0] == 'G'){
                printf("%d\n",w.get_sum(a,b));
            }
            else if(op[0] == 'M'){
                scanf("%d",&c);
                w.make_same(a,b,c);
            }
            else if(op[0] == 'R'){
                w.reverse(a,b);
            }
        }
    }
}

int main(){
    init();
    solve();
    return 0;
}