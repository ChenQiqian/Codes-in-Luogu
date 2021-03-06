#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

const int MAXN = 500000;

const int MAX = 2147483647;

int k,n;

struct node_t{
    int val,p;
    int lmax,rmax,maxn,sumn;
    int tag;
    int size;
    bool rev,is_tag;
    node_t *son[2];
    node_t **null;
    void pushdown(){
        if(this == *null)
            return;
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
        if(this == *null)
            return;
        if(son[0] == *null && son[1] == *null){
            size = 1;
            sumn = lmax = rmax = maxn = val;
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
        if(this == *null)
            return;
        val = v;sumn = size * v;
        lmax = rmax = maxn = max(v,sumn);
        is_tag = 1;tag = v;
    }
    void reverse(){
        if(this == *null)
            return;
        swap(son[0],son[1]);
        swap(lmax,rmax);
        rev^=1;
    }
};


struct fhqtreap{
    node_t pool[MAXN];
    node_t *tmp[MAXN],*stack[MAXN];
    node_t *garbage[MAXN];
    node_t *root,*null;
    int cnt,tot;
    void newnode(node_t *&r,int val = 0){
        //printf("newnode!:%d\n",val);
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
        tot = 0;
        cnt = 0;srand(time(NULL));
        newnode(null);
        null->p = 2147483647;
        root = null;
        null -> size = 0;
    }
    void cycle(node_t *r){
        if(r == null){
            return;
        }
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
        int nowp = top - 1;
        for(int i = 1;i<=n;i++){
            node_t *r = tmp[i];
            while(stack[nowp]->p > r -> p){
                stack[nowp--]->pushup();
            }
            if(nowp != top - 1){
                stack[nowp]->son[0] = stack[nowp+1];
            }
            top = ++nowp;
            stack[top++]->son[1] = r;
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
        //print(ls);print(rs);
        //printf("---------------------\n");
    }
    node_t *merge(node_t *ls,node_t *rs){
        if(ls == null) return rs;
        if(rs == null) return ls;
        //print(ls);print(rs);
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
    void insert(int val, int rank){
        node_t *ls,*rs,*newn,*ret;
        split(root,rank,ls,rs);
        newnode(newn,val);
        root = merge(merge(ls,newn),rs);
    }
    void split(int ls,int ms,node_t *&l,node_t *&m,node_t *&r){
        node_t *m1;
        split(root,ls,l,m1);
        split(m1,ms,m,r);
        //print(m);
    }
    void erase(int lb,int ms){
        node_t *l,*m,*r,*ret;
        split(lb-1,ms,l,m,r);
        cycle(m);
        root = merge(l,r);
    }
    int get_sum(int lb,int ms){
        node_t *l,*m,*r;
        split(lb-1,ms,l,m,r);
        //m->pushdown();
        int ans = m->sumn;
        root = merge(l,merge(m,r));
        return ans;
    }
    int max_sum(){
        //root->pushdown();
        return root->maxn;
    }
    void reverse(int lb,int ms){
        node_t *l,*m,*r;
        split(lb-1,ms,l,m,r);
        //m->pushdown();
        m->reverse();       
        root = merge(l,merge(m,r));
    }
    
    void make_same(int lb,int ms,int c){
        node_t *l,*m,*r;
        split(lb-1,ms,l,m,r);
        //m->pushdown();
        m->cover(c);
        root = merge(l,merge(m,r));
    }
    void print(node_t *r = NULL,int depth = 0){
        if(r == NULL) r = root;
        if(r == null) return;
        print(r->son[0],depth+1);
        
        for(int i = 0;i<depth;i++)
            putchar(' ');
        printf("val:%d p:%d size:%d son:%d %d\n",r->val,r->p,r->size,r->son[0] != null,r->son[1] != null);
        for(int i = 0;i<depth;i++)
            putchar(' ');
        printf("sum:%d maxn:%d lmax:%d rmax:%d\n",r->sumn,r->maxn,r->lmax,r->rmax);
        for(int i = 0;i<depth;i++)
            putchar(' ');
        printf("is_tag?:%d tag:%d rev?:%d\n",r->is_tag,r->tag,r->rev);        
        
        print(r->son[1],depth+1);
        return;
    }
};

fhqtreap w;

void p(){
    node_t *r = w.null;
    printf("------------------------------\n");            
    printf("null:\n");
    printf("    val:%d p:%d size:%d son:%d %d\n",r->val,r->p,r->size,r->son[0] != w.null,r->son[1] != w.null);
    printf("    sum:%d maxn:%d lmax:%d rmax:%d\n",r->sumn,r->maxn,r->lmax,r->rmax);
    printf("    is_tag?:%d tag:%d rev?:%d\n",r->is_tag,r->tag,r->rev);
    printf("------------------------------\n");
    w.print();
    printf("------------------------------\n");
}


int main(){
    freopen("1.in","r",stdin);
    //freopen("2.out","w",stdout);
    scanf("%d %d",&n,&k);
    for(int i = 1;i<=n;i++){
        int tmp;
        scanf("%d",&tmp);
        w.insert(tmp,i-1);
    }
    for(int i = 1;i<=k;i++){
        char op[50];int a,b,c;
        scanf("%s",op);
        if(op[0] == 'M'&&op[2] == 'X'){
            printf("%d\n",w.max_sum());
        }
        else if(op[0] == 'P'){
            p();
        }
        else{
            scanf("%d %d",&a,&b);
            if(op[0] == 'I'){
                for(int i = 1;i<=b;i++){
                    scanf("%d",&c);
                    w.insert(c,a+i-1);
                }
                //w.print();
            }
            else if(op[0] == 'D'){
                w.erase(a,b);
                //p();
            }
            else if(op[0] == 'G'){
                printf("%d\n",w.get_sum(a,b));
            }
            else if(op[0] == 'M'){
                scanf("%d",&c);
                w.make_same(a,b,c);
                //p();
            }
            else if(op[0] == 'R'){
                w.reverse(a,b);
                //p();
            }
            else{
                printf("oops.There is something wrong.\n");
            }
        }
        //for(int i = 1;i<=w.root->size;i++){
        //    w.get_sum(i,1);
        //}
        //w.print();
    }
}