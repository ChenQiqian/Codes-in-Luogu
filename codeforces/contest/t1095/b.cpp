#include <bits/stdc++.h>
using namespace std;

const int MAXN = 210000;
int n,a[MAXN];

int main(){
  scanf("%d",&n);
  for(int i = 1;i<=n;i++){
    scanf("%d",&a[i]);
  }
  sort(a+1,a+n+1);
  printf("%d\n",min(a[n-1] - a[1],a[n]-a[2]));
  return 0;
}