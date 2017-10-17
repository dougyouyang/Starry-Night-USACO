//  Copyright © 2017 Dougy Ouyang. All rights reserved.

#include <iostream>
#include <queue>
#include <vector>

using namespace std;
char inputgraph[102][102];
int n, m, graph[102][102], total=0;

struct cluster {
    int num, dimx, dimy, body[101][101];
};
vector<cluster> uniquec;

void floodfill()
{
    const int dir[8][2]={{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    int i, j, k;
    for(i=1;i<=n;i++)
        for(j=1;j<=m;j++)
            if(inputgraph[i][j]=='*'){
                total++;
                queue<pair<int, int> > po;
                pair<int, int> in, out;
                in.first=i, in.second=j, po.push(in);
                graph[i][j]=total, inputgraph[i][j]='.';
                while(!po.empty()){
                    out=po.front(), po.pop();
                    for(k=0;k<8;k++)
                        if(inputgraph[out.first+dir[k][0]][out.second+dir[k][1]]=='*')
                            in.first=out.first+dir[k][0], in.second=out.second+dir[k][1], po.push(in), inputgraph[in.first][in.second]='.', graph[in.first][in.second]=total;
                }
            }
}

void getc(int x, cluster &clu)
{
    int i, j;
    for(i=0;i<=100;i++)
        for(j=0;j<=100;j++)
            clu.body[i][j]=0;
    for(i=1;i<=n;i++)
        for(j=1;j<=m;j++)
            if(graph[i][j]==x)
                clu.body[i][j]=1;
}

void processc(cluster &clu)
{
    int minx=101, maxx=0, miny=101, maxy=0, t=0;
    int i, j;
    
    for(i=1;i<=n;i++)
        for(j=1;j<=m;j++)
            if(clu.body[i][j])
                minx=min(minx, i), maxx=max(maxx, i), miny=min(miny, j), maxy=max(maxy, j), t++;
    clu.num=t, clu.dimx=maxx-minx+1, clu.dimy=maxy-miny+1;
    for(i=minx;i<=maxx;i++)
        for(j=miny;j<=maxy;j++)
            clu.body[i-minx][j-miny]=clu.body[i][j];
}

void r90(cluster &clu1, cluster &clu2)
{
    clu2=clu1;
    clu2.dimx=clu1.dimy, clu2.dimy=clu1.dimx;
    int i, j;
    for(i=0;i<clu1.dimx;i++)
        for(j=0;j<clu1.dimy;j++)
            clu2.body[j][i]=clu1.body[i][j];
}

void r180(cluster &clu1, cluster &clu2)
{
    clu2=clu1;
    int i, j;
    for(i=0;i<clu1.dimx;i++)
        for(j=0;j<clu1.dimy;j++)
            clu2.body[i][j]=clu1.body[clu1.dimx-i-1][clu1.dimy-j-1];
}

void r270(cluster &clu1, cluster &clu2)
{
    clu2=clu1;
    clu2.dimx=clu1.dimy, clu2.dimy=clu1.dimx;
    int i, j;
    for(i=0;i<clu1.dimx;i++)
        for(j=0;j<clu1.dimy;j++)
            clu2.body[j][i]=clu1.body[i][clu1.dimy-j-1];
}

void ref(cluster &clu1, cluster &clu2)
{
    clu2=clu1;
    int i, j;
    for(i=0;i<clu1.dimx;i++)
        for(j=0;j<clu1.dimy;j++)
            clu2.body[i][j]=clu1.body[i][clu1.dimy-j-1];
}

void trans(int idx, cluster &clu1, cluster &clu2)
{
    cluster temc;
    
    switch(idx) {
        case 1:
            clu2=clu1;
            break;
        case 2:
            r90(clu1, clu2);
            break;
        case 3:
            r180(clu1, clu2);
            break;
        case 4:
            r270(clu1, clu2);
            break;
        case 5:
            ref(clu1, clu2);
            break;
        case 6:
            r90(clu1, temc), ref(temc, clu2);
            break;
        case 7:
            r180(clu1, temc), ref(temc, clu2);
            break;
        case 8:
            r270(clu1, temc), ref(temc, clu2);
            break;
    }
}

int match(cluster &clu1, cluster &clu2)
{
    if(clu1.num!=clu2.num || clu1.dimx!=clu2.dimx || clu1.dimy!=clu2.dimy)
        return 0;
    int i, j;
    for(i=0;i<clu1.dimx;i++)
        for(j=0;j<clu1.dimy;j++)
            if(clu1.body[i][j]!=clu2.body[i][j])
                return 0;
    return 1;
}

int main()
{
    int i, j, k;
    cluster clu, tranclu, matchclu;
    
    cin >> n >> m;
    for(i=1;i<=n;i++){
        string input;
        cin >> input;
        for(j=1;j<=m;j++)
            inputgraph[i][j]=input[j-1];
    }
    
    floodfill();
    for(i=1;i<=total;i++){
        getc(i, clu);
        processc(clu);
        bool found=false;
        for(j=1;j<=uniquec.size();j++){
            if(found)
                break;
            matchclu=uniquec[j-1];
            for(k=1;k<=8;k++){
                trans(k, clu, tranclu);
                if(match(tranclu, matchclu)){
                    found=true;
                    break;
                }
            }
        }
        if(!found)
            uniquec.push_back(clu);
    }
    
    cout << uniquec.size() << endl;
    
    return 0;
}
