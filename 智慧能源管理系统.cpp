#include<iostream>
#include<cstdio>
#include<vector>
#include<queue>
#include<stack>
#include<vector>
#include<Windows.h>
#define MaxVertexNum 100  /* 最大顶点数设为100 */
#include <fstream>
using namespace std;

const int INF = 0x3f3f3f3f;
const int inf = 99999;   
typedef int Vertex;         /* 用顶点下标表示顶点,为整型 */
typedef int WeightType;        /* 边的权值设为整型 */
typedef char DataType;        /* 顶点存储的数据类型设为字符型 */
int begin_node = 0;

/* 边的定义 */
typedef struct ENode* PtrToENode;
struct ENode {
	Vertex V1, V2;      /* 有向边<V1, V2> */
	WeightType Weight;  /* 权重 */
};
typedef PtrToENode Edge;

/* 邻接点的定义 */
typedef struct AdjVNode* PtrToAdjVNode;
struct AdjVNode {
	Vertex AdjV;        /* 邻接点下标 */
	WeightType Weight;  /* 边权重 */
	PtrToAdjVNode Next;    /* 指向下一个邻接点的指针 */
};

/* 顶点表头结点的定义 */
typedef struct Vnode {
	PtrToAdjVNode FirstEdge;/* 边表头指针 */
	DataType Data[100];            /* 存顶点的数据 */
	/* 注意：很多情况下，顶点无数据，此时Data可以不用出现 */
} AdjList[MaxVertexNum];    /* AdjList是邻接表类型 */

/* 图结点的定义 */
typedef struct GNode* PtrToGNode;
struct GNode {
	int Nv;     /* 顶点数 */
	int Ne;     /* 边数   */
	AdjList G;  /* 邻接表 */
};
typedef PtrToGNode LGraph; /* 以邻接表方式存储的图类型 */

struct node
{
	int id;
	int dis;
	friend bool operator<(const node& a, const node& b) {
		return a.dis > b.dis;
	}
};



LGraph Graph;
int dist[MaxVertexNum],path[MaxVertexNum];
bool vis[MaxVertexNum];
int prevv[100],m;




//函数声明模块->
void menu();
void search(LGraph Graph);
void cal_min_route(LGraph Graph,int s,int prevv[],int dist[]);
void infor_inserc(LGraph Graph);
void delete_points(LGraph Graph,char name[50]);
int getPosition(LGraph Graph, char name[50]);
int get_weight(LGraph Graph, int start, int end);
void two_point_route(LGraph Graph, char name1[50], char name2[50], int prevv[], int dist[]);
void menu1();
void change_data(LGraph Graph, char name1[], char name2[]);
void modifyEdgeWeight(LGraph Graph, Vertex start, Vertex end, WeightType newWeight);
LGraph CreateGraph(int VertexNum);
void InsertEdge(LGraph Graph, Edge E);
LGraph BuildGraph();
void menu();
void menu1();
void saveToFile(LGraph Graph, const char* filename);
LGraph readFromFile(const char* filename);
//函数声明模块<-


int main()
{

	const char* filename = "graph_data.txt";
    const char* loadFilename = "graph_data.txt";
	while(1)
	{
		menu();
		int x=0;
		cout << "请输入操作编号:" << endl;
		cin >> x;
		switch (x)
		{
			case 1:Graph = BuildGraph();
				break;

			case 2:search(Graph); break;

			case 3:system("cls"); menu1(); 
					int button; 
					cout << "请输入对应操作序号:";
					cin >> button;
					if (button == 1) {
						char name[50];
						cout << "想要删除的节点名称" << endl;
						cin >> name;
						delete_points(Graph, name);
					}
					else if (button == 2) {
						char name[50];
						char after_name[50];
						cout << "想要修改的节点名称" << endl;
						cin >> name;
						getchar();
						cout << "请输入修改后的名称" << endl;
						cin >> after_name;
						getchar();
						change_data(Graph, name, after_name);
					}
					else if (button==3)
					{
						infor_inserc(Graph);
					}
					else
					{
						int start, end;
						char name1[50], name2[20];
						WeightType newWeight;
						cout << "请输入需要修改边的起点产品名称:";
						cin >> name1;
						getchar();
						cout << endl;
						cout << "请输入需要修改边的终点产品名称:";
						cin >> name2;
						getchar();
						cout << endl;
						cout << "请输入新的权重: ";
						cin >> newWeight;
						start = getPosition(Graph, name1);
						end = getPosition(Graph, name2);
						modifyEdgeWeight(Graph, start, end, newWeight);
					}
					break;
			case 4:
				int s; char name[50];
				cout << "请输入起始产品名称："; 
				cin >> name;
				s = getPosition(Graph, name);
				cal_min_route(Graph,s,prevv,dist); 
				break;
			case 5:
				char need_name1[50],need_name2[50];
				cout << "调节能源分配，车间优先使用规定产品，生成目标产品" << endl;
				cout << "请输入起点产品名称："<<endl;
				cin >> need_name1; 
				getchar();
				cout << "请输入目标产品名称："<<endl;
				cin >> need_name2;
				getchar();
				two_point_route(Graph, need_name1, need_name2, prevv, dist);
				break;
			case 6:
               // const char* filename = "graph_data.txt";
                saveToFile(Graph, filename);
                break;
            case 7:
               // const char* loadFilename = "graph_data.txt";
                Graph = readFromFile(loadFilename);
                break;
			case 0:
				cout << "即将关闭程序" << endl;
				goto a;

			default:
				cout << "输入操作不正确，请重新输入" << endl;
				system("pause");
				system("cls");
		}
	}
	a:system("pause");
	return 0;
}


//查询该原料生成的产物->
void search(LGraph Graph)
{
	char name[50];
	int save = 0;
	cout << "请输入查询此产品的生成产品:" << endl;
	cin >> name;
	for (int i = 0; i < Graph->Nv; i++)
	{
		if (!strcmp(name, Graph->G[i].Data))
		{
			save = i;
			break;
		}
	}
	PtrToAdjVNode p = Graph->G[save].FirstEdge;
	if (!p)
	{
		cout << "该产品无生产产品" << endl;
		return;
	}

	while (p)
	{

		cout << Graph->G[p->AdjV].Data<<" ";
		p = p->Next;
	}
	cout << endl;
}
//查询该原料生成的产物<-

//计算最短路->
void cal_min_route(LGraph G,int vs,int prevv[],int dist[])
{
	int i, j, k, t, m;
	int minResult, tmp, flag[inf], path[MaxVertexNum][MaxVertexNum] = { 0 };
	for (i = 0; i < Graph->Nv; i++)
	{
		flag[i] = 0;
		prevv[i] = -1;
		dist[i] = get_weight(Graph, vs, i);
		for (j = 0; j < MaxVertexNum; j++)
		{
			path[i][j] = -1;
		}
	}

	flag[vs] = 1;
	dist[vs] = 0;
	path[vs][0] = 1;
	for (i = 1; i < Graph->Nv; i++)
	{
		t = 0;
		minResult = INF;
		for (j = 0; j < Graph->Nv; j++)
		{
			if (flag[j] == 0 && dist[j] < minResult)
			{
				minResult = dist[j];
				k = j;
			}
		}

		flag[k] = 1;
		for (j = 0; j < Graph->Nv; j++)
		{
			tmp = get_weight(Graph, k, j);
			tmp = (tmp == INF ? INF : (minResult + tmp));
			if (flag[j] == 0 && (tmp < dist[j]))
			{
				dist[j] = tmp;
				prevv[j] = k;
				/*path[j][t] = k;
				t++;*/
				for (m = 0; m < MaxVertexNum; m++)
				{
					if (path[j][m] == -1)
					{
						path[j][m] = k;
						break;
					}
				}
			}
		}
	}

	cout << "  从产品" << Graph->G[vs].Data << "到其他产品的最少能源消耗：\n";
	for (int i = 0; i < Graph->Nv; i++) {
		if (strcmp(Graph->G[i].Data, Graph->G[vs].Data) != 0) {
			cout << "   " << Graph->G[vs].Data << "到" << Graph->G[i].Data << "的最少能源消耗为：" << endl;
			if (dist[i] != INF) {
				cout << "    " << Graph->G[vs].Data;
				for (int q = MaxVertexNum - 1; q >= 0; q--) {
					if (path[i][q] == -1)
						continue;
					cout << "->" << Graph->G[path[i][q]].Data;
				}
				cout << "->" << Graph->G[i].Data << "，能源消耗为：" << dist[i] << endl;
			}
			else {
				cout << "   无生产关系" << endl;
			}
		}
	}
}
//计算最短路<-

//数据动态插入->
void infor_inserc(LGraph Graph)
{
	int num_points = Graph->Nv;
	int num_edges = Graph->Ne;
	int num_need_points=0;
	int num_need_edges = 0;
	cout << "需要插入的节点个数:";
	cin >> num_need_points;
	getchar();
	for (int i = 0; i < num_need_points; i++)
		Graph->G[i + num_points].FirstEdge = NULL;
	Graph->Nv = num_points + num_need_points;
	cout << "目前系统中拥有的产品数：" << Graph->Nv << endl;
	
	
	for (int i = 0; i < num_need_points; i++)
	{
		cout << "请输入产品编号"<<num_points+i<<"的产品名称" << ":" << endl;
		cin >> Graph->G[i+num_points].Data;
		getchar();
	}

	cout << "需要插入的边数：";
	cin >> num_need_edges;
	for (int i = 0; i < num_need_edges; i++)
	{
		char A[50], B[50];
		PtrToENode p = new ENode;
		cout << "依次输入起点、终点、权重：";
		cin >> A; getchar(); cin >> B; getchar(); cin >> p->Weight;
		p->V1 = getPosition(Graph, A);
		p->V2 = getPosition(Graph, B);
		PtrToAdjVNode NewNode = new AdjVNode;
		NewNode->AdjV = p->V2;
		NewNode->Weight = p->Weight;
		NewNode->Next = Graph->G[p->V1].FirstEdge;
		Graph->G[p->V1].FirstEdge = NewNode;
	}
	Graph->Ne += num_need_edges;
	return;
}
//数据动态插入<-


int getPosition(LGraph Graph, char name[50])
{
	int m;
	for (m = 0; m < Graph->Nv; m++)
		if (strcmp(name,Graph->G[m].Data)==0)
			return m;
	return -1;
}

int get_weight(LGraph Graph, int start, int end)
{
	PtrToAdjVNode node;
	if (start == end)
		return 0;
	node = Graph->G[start].FirstEdge;
	while (node!=NULL)
	{
		if (end == node->AdjV)
			return node->Weight;
		node = node->Next;
	}
	return INF;
}


//数据删除-》
void delete_points(LGraph Graph, char name[50])
{
    int k = -1, cnt = 0;

    // 查找待删除节点的位置
    for (int i = 0; i < Graph->Nv; i++)
    {
        if (!strcmp(Graph->G[i].Data, name))
        {
            k = i;
            break;
        }
    }

    if (k == -1)
    {
        cout << "未找到要删除的产品：" << name << endl;
        return;
    }

    // 删除与该节点相关的边
    for (int i = 0; i < Graph->Nv; i++)
    {
        PtrToAdjVNode p = Graph->G[i].FirstEdge;
        PtrToAdjVNode q = NULL;
        while (p)
        {
            if (p->AdjV == k)
            {
                PtrToAdjVNode tmp = p;
                q ? (q->Next = p->Next) : (Graph->G[i].FirstEdge = p->Next);
                p = p->Next;
                delete tmp;
                cnt++;
            }
            else
            {
                q = p;
                p = p->Next;
            }
        }
    }

    // 删除该节点
    for (int i = k + 1; i < Graph->Nv; i++)
    {
        Graph->G[i - 1] = Graph->G[i];

        // 更新其他节点的邻接表中的节点编号
        PtrToAdjVNode p = Graph->G[i - 1].FirstEdge;
        while (p)
        {
            if (p->AdjV > k)
                p->AdjV--;
            p = p->Next;
        }
    }

    // 修正边数
    Graph->Ne -= cnt;

    Graph->Nv--;
    cout << "产品 " << name << " 已成功删除。" << endl;
}

//数据删除《-


void two_point_route(LGraph Graph, char name1[50], char name2[50], int prevv[], int dist[])
{
	int x = getPosition(Graph, name1);
	int y = getPosition(Graph, name2);
	if (x == -1 || y == -1)
	{
		cout << "你输出的产品名称不在系统中，请查询后重新输入正确产品名称" << endl;
		return;
	}
	int i, j, k, t, m;
	int minResult, tmp, flag[inf], path[MaxVertexNum][MaxVertexNum] = { 0 };
	for (i = 0; i < Graph->Nv; i++)
	{
		flag[i] = 0;
		prevv[i] = -1;
		dist[i] = get_weight(Graph, x, i);
		for (j = 0; j < MaxVertexNum; j++)
		{
			path[i][j] = -1;
		}
	}

	flag[x] = 1;
	dist[x] = 0;
	path[x][0] = 1;
	for (i = 1; i < Graph->Nv; i++)
	{
		t = 0;
		minResult = INF;
		for (j = 0; j < Graph->Nv; j++)
		{
			if (flag[j] == 0 && dist[j] < minResult)
			{
				minResult = dist[j];
				k = j;
			}
		}

		flag[k] = 1;
		for (j = 0; j < Graph->Nv; j++)
		{
			tmp = get_weight(Graph, k, j);
			tmp = (tmp == INF ? INF : (minResult + tmp));
			if (flag[j] == 0 && (tmp < dist[j]))
			{
				dist[j] = tmp;
				prevv[j] = k;
				/*path[j][t] = k;
				t++;*/
				for (m = 0; m < MaxVertexNum; m++)
				{
					if (path[j][m] == -1)
					{
						path[j][m] = k;
						break;
					}
				}
			}
		}
	}
	for (int i = 0; i < Graph->Nv; i++) {
		if (strcmp(Graph->G[i].Data, name2) == 0) {
			cout << "~~~~~" << Graph->G[x].Data << "到" << Graph->G[i].Data << "的能源消耗为：" << endl;
			if (dist[i] != INF) {
				cout << "    " << Graph->G[x].Data;
				for (int q = MaxVertexNum - 1; q >= 0; q--) {
					if (path[i][q] == -1)
						continue;
					cout << "->" << Graph->G[path[i][q]].Data;
				}
				cout << "->" << Graph->G[i].Data << "，能源消耗为：" << dist[i] << endl;
			}
			else {
				cout << "   无法生成" << endl;
			}
		}
	}
	cout << "请按上述路线调整能源分配关系,以保证最少能源消耗" << endl;
}


void change_data(LGraph Graph, char name1[], char name2[])
{
	int l = getPosition(Graph, name1);
	if (l == -1)
	{
		cout << "你输出的产品名称不在系统中，请查询后重新输入正确产品名称" << endl;
		return;
	}
	cout << "产品编号:" << l<<endl;
	strcpy(Graph->G[l].Data, name2);
	cout << "重新修改产品名称完成" << endl;
	cout << Graph->G[l].Data << endl;
}

// 修改边的权重
void modifyEdgeWeight(LGraph Graph, Vertex start, Vertex end, WeightType newWeight) {
	// 找到起点 start 对应的链表头
	PtrToAdjVNode p = Graph->G[start].FirstEdge;

	// 遍历链表，找到终点为 end 的边
	while (p != NULL && p->AdjV != end) {
		p = p->Next;
	}

	// 如果找到了对应的边，修改权重
	if (p != NULL) {
		p->Weight = newWeight;
		cout << "成功修改生产关系 (" << Graph->G[start].Data << ", " << Graph->G[end].Data << ") 的能源消耗为 " << newWeight << endl;
	}
	else {
		cout << "未找到相关产品生产关系 (" << Graph->G[start].Data << ", " << Graph->G[end].Data << ")，无法修改能源消耗" << endl;
	}
}



LGraph CreateGraph(int VertexNum)
{ /* 初始化一个有VertexNum个顶点但没有边的图 */
	Vertex V;
	LGraph Graph;

	Graph = (LGraph)malloc(sizeof(struct GNode)); /* 建立图 */
	Graph->Nv = VertexNum;
	Graph->Ne = 0;
	/* 初始化邻接表头指针 */
	/* 注意：这里默认顶点编号从0开始，到(Graph->Nv - 1) */
	for (V = 0; V < Graph->Nv; V++)
	{
		Graph->G[V].FirstEdge = NULL;
	}

	return Graph;
}

void InsertEdge(LGraph Graph, Edge E)
{
	PtrToAdjVNode NewNode;

	/* 插入边 <V1, V2> */
	/* 为V2建立新的邻接点 */
	NewNode = (PtrToAdjVNode)malloc(sizeof(struct AdjVNode));
	NewNode->AdjV = E->V2;
	NewNode->Weight = E->Weight;
	/* 将V2插入V1的表头 */
	NewNode->Next = Graph->G[E->V1].FirstEdge;
	Graph->G[E->V1].FirstEdge = NewNode;
}

LGraph BuildGraph()
{
	LGraph Graph;
	Edge E;
	Vertex V;
	int Nv, i;
	cout << "请输入顶点个数:";
	cin >> Nv;  /* 读入顶点个数 */
	Graph = CreateGraph(Nv); /* 初始化有Nv个顶点但没有边的图 */

	getchar();
	cout << "请输入产品名称：" << endl;
	/* 如果顶点有数据的话，读入数据 */
	for (V = 0; V < Graph->Nv; V++)
	{
		cout << "请输入产品编号为" << V << "的产品名称:";
		cin >> Graph->G[V].Data;
		getchar();
	}

	cout << "请输入边数：";
	cin >> Graph->Ne;
	// scanf("%d", &(Graph->Ne));   /* 读入边数 */
	if (Graph->Ne != 0) { /* 如果有边 */
		E = (Edge)malloc(sizeof(struct ENode)); /* 建立边结点 */
		/* 读入边，格式为"起点 终点 权重"，插入邻接矩阵 */
		for (i = 0; i < Graph->Ne; i++) {
			char A[50], B[50];
			cout << "依次输入起点产品名称、终点产品名称、权重："<<endl;
			cin >> A; getchar(); cin >> B; getchar(); cin >> E->Weight;
			E->V1 = getPosition(Graph, A);
			E->V2 = getPosition(Graph, B);
			 /* 注意：如果权重不是整型，Weight的读入格式要改 */
			InsertEdge(Graph, E);
		}
	}
	return Graph;
}

void menu()
{
	cout << "------------------------" << endl;
	cout << "*   智慧能源管理系统   *" << endl;
	cout << "------------------------" << endl;
	cout << "*      1.初始能源数据  *" << endl;
	cout << "*      2.能源数据查询  *" << endl;
	cout << "*      3.动态数据操作  *" << endl;
	cout << "*      4.最优能源结构  *" << endl;
	cout << "*      5.调整能源生产  *" << endl;
	cout << "*      6.保存文件      *" << endl;
	cout << "*      7.读取文件      *" << endl;
	cout << "*      0.关闭程序      *" << endl;
	cout << "*----------------------*" << endl;
}

void menu1()
{
	cout << "------------------------" << endl;
	cout << "|      动态数据操作    |" << endl;
	cout << "------------------------" << endl;
	cout << "|      1.数据删除      |" << endl;
	cout << "|      2.数据修改      |" << endl;
	cout << "|      3.数据添加      |" << endl;
	cout << "|      4.边权修改      |" << endl;
	cout << "|----------------------|" << endl;
}

void saveToFile(LGraph Graph, const char* filename) {
    std::ofstream outFile(filename);

    if (!outFile.is_open()) {
        std::cerr << "无法打开文件：" << filename << std::endl;
        return;
    }
	cout<<"产品数:"<<Graph->Ne<<"产品关系数:"<<Graph->Nv<<endl;
    // 保存顶点信息
    outFile << Graph->Nv << std::endl;
    for (int i = 0; i < Graph->Nv; i++) {
        outFile << Graph->G[i].Data << std::endl;
    }

    // 保存边信息
    outFile << Graph->Ne << std::endl;
    for (int i = 0; i < Graph->Nv; i++) {
        PtrToAdjVNode p = Graph->G[i].FirstEdge;
        while (p) {
            outFile << Graph->G[i].Data << " " << Graph->G[p->AdjV].Data << " " << p->Weight << std::endl;
            p = p->Next;
        }
    }

    outFile.close();
    std::cout << "图数据已保存到文件：" << filename << std::endl;
}

// 从文件读取图数据
LGraph readFromFile(const char* filename) {
    std::ifstream inFile(filename);

    if (!inFile.is_open()) {
        std::cerr << "无法打开文件：" << filename << std::endl;
        return nullptr;
    }
	
    int Nv, Ne;
    inFile >> Nv; // 读取顶点数
    LGraph Graph = CreateGraph(Nv);

    // 读取顶点信息
    for (int i = 0; i < Nv; i++) {
        inFile >> Graph->G[i].Data;
    }

    // 读取边信息
    inFile >> Ne; // 读取边数
    Graph->Ne=Ne;
    for (int i = 0; i < Ne; i++) {
        char A[50], B[50];
        WeightType Weight;
        inFile >> A >> B >> Weight;
        Vertex V1 = getPosition(Graph, A);
        Vertex V2 = getPosition(Graph, B);
        Edge E = new ENode{ V1, V2, Weight };
        InsertEdge(Graph, E);
    }

    inFile.close();
    std::cout << "图数据已从文件加载：" << filename << std::endl;

    return Graph;
}
