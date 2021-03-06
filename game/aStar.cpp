
#ifndef _aStar_H_
    #define _aStar_H_

#include "main.h"


inline bool operator<(const node & a, const node & b)
{
  return a.getPriority() > b.getPriority();

}

//todo:: use same vector instead of array
//test
void CApp::getMap()
{
	for (int i = 0, p = 0; i < mapHeight*mapPerAreaY; i++)
	{
		for (int j = 0; j < mapWidth*mapPerAreaX; j++)
		{
			cout << cC.mapInfoCollision[p++];
		}
		cout << endl;
	}
}

string CApp::pathFind( const int & xStart, const int & yStart, 
                 const int & xFinish, const int & yFinish )
{
	//new
	vector<vector<int>> closed_nodes_map;
	closed_nodes_map.resize(mapHeight*mapPerAreaY, vector<int>(mapWidth*mapPerAreaX, 0));

	vector<vector<int>> open_nodes_map;
	open_nodes_map.resize(mapHeight*mapPerAreaY, vector<int>(mapWidth*mapPerAreaX, 0));

	vector<vector<int>> dir_map;
	dir_map.resize(mapHeight*mapPerAreaY, vector<int>(mapWidth*mapPerAreaX, 0));

	//We need to define a maximum range of path finding so it either doesn't do an infinate loop or takes too long to find the path
	unsigned int counter;
	//new
	
	static priority_queue<node> pq[2]; // list of open (not-yet-tried) nodes
    static int pqi; // pq index
    static node* n0;
    static node* m0;
    static int i, j, x, y, xdx, ydy;
    static char c;
    pqi=0;

    // create the start node and push into list of open nodes
    n0=new node(xStart, yStart, 0, 0);
    n0->updatePriority(xFinish, yFinish);
    pq[pqi].push(*n0);
    open_nodes_map[x][y]=n0->getPriority(); // mark it on the open nodes map
	
    // A* search
    while(!pq[pqi].empty())
    {	
        // get the current node w/ the highest priority
        // from the list of open nodes
        n0=new node( pq[pqi].top().getxPos(), pq[pqi].top().getyPos(), 
                     pq[pqi].top().getLevel(), pq[pqi].top().getPriority());
		
        x=n0->getxPos(); y=n0->getyPos();
		
        pq[pqi].pop(); // remove the node from the open list
        open_nodes_map[x][y]=0;
        // mark it on the closed nodes map
        closed_nodes_map[x][y]=1;
		
        // quit searching when the goal state is reached
        //if((*n0).estimate(xFinish, yFinish) == 0)
        if(x==xFinish && y==yFinish) 
        {	
            // generate the path from finish to start
            // by following the directions
            string path="";
            while(!(x==xStart && y==yStart))
            {
                j=dir_map[x][y];
                c='0'+(j+dir/2)%dir;
                path=c+path;
                x+=dx[j];
                y+=dy[j];
				//if(path.size() > 20)
				//	 return "";
				
            }

            // garbage collection
            delete n0;
            // empty the leftover nodes
            while(!pq[pqi].empty()) pq[pqi].pop();           
            return path;
        }

        // generate moves (child nodes) in all possible directions
        for(i=0;i<dir;i++)
        {	
            xdx=x+dx[i]; ydy=y+dy[i];

			//changed (works):
			//cout << cC.mapInfoCollision[xdx + ydy*mapWidth] << " ";
            //if(!(xdx<0 || xdx>n-1 || ydy<0 || ydy>m-1 || map[xdx][ydy]==1 
			if (!(xdx<0 || xdx>n - 1 || ydy<0 || ydy>m - 1 || cC.mapInfoCollision[xdx + ydy*mapWidth*mapPerAreaX] == 1
                || closed_nodes_map[xdx][ydy]==1))
            {
				
                // generate a child node
                m0=new node( xdx, ydy, n0->getLevel(), 
                             n0->getPriority());
                m0->nextLevel(i);
                m0->updatePriority(xFinish, yFinish);

                // if it is not in the open list then add into that
                if(open_nodes_map[xdx][ydy]==0)
                {
						
                    open_nodes_map[xdx][ydy]=m0->getPriority();
                    pq[pqi].push(*m0);
                    // mark its parent node direction
                    dir_map[xdx][ydy]=(i+dir/2)%dir;
                }
                else if(open_nodes_map[xdx][ydy]>m0->getPriority())
                {
						
                    // update the priority info
                    open_nodes_map[xdx][ydy]=m0->getPriority();
                    // update the parent direction info
                    dir_map[xdx][ydy]=(i+dir/2)%dir;

                    // replace the node
                    // by emptying one pq to the other one
                    // except the node to be replaced will be ignored
                    // and the new node will be pushed in instead
                    while(!(pq[pqi].top().getxPos()==xdx && 
                           pq[pqi].top().getyPos()==ydy))
                    {                
                        pq[1-pqi].push(pq[pqi].top());
                        pq[pqi].pop();       
                    }
                    pq[pqi].pop(); // remove the wanted node
                    
                    // empty the larger size pq to the smaller one
                    if(pq[pqi].size()>pq[1-pqi].size()) pqi=1-pqi;
                    while(!pq[pqi].empty())
                    {                
                        pq[1-pqi].push(pq[pqi].top());
                        pq[pqi].pop();       
                    }
                    pqi=1-pqi;
                    pq[pqi].push(*m0); // add the better node instead
                }
               delete m0;
					
            }
        }
        delete n0; // garbage collection
			
    }
	
    return ""; // no route found
}

#endif