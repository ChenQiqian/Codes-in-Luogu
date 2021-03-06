#include <cstdio>
#define MAX 0x3f3f3f3f
using namespace std;

inline int qr(){
    int f = 1,s = 0;char ch = getchar();
    while(ch<'0'||ch>'9'){
        if(ch == '-') f = -1;
        ch = getchar();
    }
    while(ch>='0'&&ch<='9'){
        s = (s<<3)+(s<<1)+ch-48;
        ch = getchar();
    }
    return f*s;
}

struct splay_t{
    struct node_t{
        int val,size,cnt;
        node_t *son[2],*p;node_t **null,**root;
        //与父亲关系
        inline bool get_p(){return p->son[1] == this;}
        //双向连接
        inline void link(node_t *dst,bool re){p = dst;dst->son[re] = this;}
        //更新size值
        inline void update(){size = son[0]->size + son[1]->size + cnt;}
        //初始化**root和**null
        inline void init(node_t **null,node_t **root){this->null = null,this->root = root;}
        //获取左右节点的大小
        inline int lsize(){return son[0]->size;}int rsize(){return son[1]->size;}
        //寻找节点前驱或者后继
        node_t *uporlow(int tmp){//0前驱，1后继
            splay();
            node_t *t = son[tmp];
            while(t->son[1-tmp] != *null)
                t = t->son[1-tmp]; 
            return t;
        }
        //旋转
        void rotate(){
            bool re = get_p();node_t *rp = p;
            link(rp->p,rp->get_p());
            son[1-re]->link(rp,re);
            rp->link(this,1-re);
            rp->update();update();
            if(p == *null) *root = this; 
        }
        //splay操作
        node_t* splay(node_t *tar = NULL){
            if(this == *null) return this;
            if(tar == NULL) tar = *null;
            while(p!=tar){
                if(p->p == tar) rotate();
                else{
                    if(p->get_p()==get_p()) p->rotate(),rotate();
                    else rotate(),rotate();
                }
            }
            return this;
        }
    };
    int treecnt;
    node_t pool[300000];
    node_t *null,*root,*lb,*rb;//lb是左边的虚拟节点，rb同理
    //初始化
    splay_t(){
        treecnt = 0;
        newnode(null);root = null;
        null->size = 0,null->val = 0;
        lb = insert(-MAX);rb = insert(MAX);
    }
    //新建节点
    void newnode(node_t *&r,int val = 0){
        r = &pool[treecnt++];
        r->val = val;
        r->son[0] = r->son[1] = r->p = null;
        r->cnt = r->size = 1;
        r->init(&null,&root);
    }
    //寻找给定rank的数字
    node_t* find_Kth(int rank){
        node_t *t = root;
        while(t!=null){
            if(rank<t->lsize())
                t = t->son[0];
            else if((rank-=t->lsize())<t->cnt)
                return t->splay();
            else
                rank-=t->cnt,t = t->son[1];
        }
        return null;
    }
    //按值寻找
    node_t *find_by_val(int val){
        node_t *t = root;
        while(t!=null){
            if(val<t->val)
                t = t->son[0];
            else if(val==t->val)
                return t->splay();
            else
                t = t->son[1];
        }
        return null;
    }
    //插入给定值的节点
    node_t* insert(int val){
        node_t **tar = &root,*parent = null;
        while(*tar!=null){
            (*tar)->size++;
            if((*tar)->val == val){
                (*tar)->cnt++;return *tar;
            }
            else{
                parent = *tar;tar = &(*tar)->son[(*tar)->val<val];
            }
        }
        newnode(*tar,val);
        (*tar)->link(parent,parent->val < val);
        return (*tar)->splay();
    }
    //调试用打印树
    void print(node_t *r = NULL,int depth = 0){
        if(r == NULL) r = root;
        if(r == null) return;
        else{
            print(r->son[0],depth+1);
            for(int i = 0;i<depth;i++) putchar(' ');
            printf("v:%d,size:%d,cnt:%d,son:%d %d,depth:%03d\n",r->val,r->size,r->cnt,r->son[0]!=null,r->son[1]!=null,depth);
            print(r->son[1],depth+1);
        }
    }
};

splay_t x;int n,minn,res = 0,nowadd = 0;

//插入一个数
inline void insert(int val){if(val>=minn) x.insert(val-nowadd);}//注意要减去nowadd 
//统一加工资
inline void add(int val){nowadd+=val;}
//统一减公司顺便裁人
inline void decrease(int val){
    nowadd-=val;
    splay_t::node_t *r = x.find_by_val(minn-nowadd);//注意要减去nowadd 
    if(r!=x.null)
        r->splay(),res+=(x.root->lsize()-1);
    else
        x.insert(minn-nowadd-1)->uporlow(1)->splay(),res+=(x.root->lsize()-2);
    x.lb->link(x.root,0);x.lb->son[1] = x.null;
    x.root->update();
}
//查找工资排名K位的员工的工资
inline int ask(int rank){
    if(rank > x.root->size - 2) return -1;
    return x.find_Kth(x.root->size-rank-1)->val + nowadd;//注意要加上nowadd 
}

int main(){
    n = qr();minn = qr();
    for(int i = 0;i<n;i++){
        char op[20];int k;
        scanf("%s",op);k = qr();
        if(op[0] == 'A')      add(k);
        else if(op[0] == 'S') decrease(k);
        else if(op[0] == 'I') insert(k);
        else if(op[0] == 'F') printf("%d\n",ask(k));
        else if(op[0] == 'P') x.print();
    }
    printf("%d\n",res);
    return 0;
}