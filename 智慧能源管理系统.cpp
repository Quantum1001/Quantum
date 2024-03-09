#include<iostream>
#include<cstdio>
#include<vector>
#include<queue>
#include<stack>
#include<vector>
#include<Windows.h>
#define MaxVertexNum 100  /* ��󶥵�����Ϊ100 */
#include <fstream>
using namespace std;

const int INF = 0x3f3f3f3f;
const int inf = 99999;   
typedef int Vertex;         /* �ö����±��ʾ����,Ϊ���� */
typedef int WeightType;        /* �ߵ�Ȩֵ��Ϊ���� */
typedef char DataType;        /* ����洢������������Ϊ�ַ��� */
int begin_node = 0;

/* �ߵĶ��� */
typedef struct ENode* PtrToENode;
struct ENode {
	Vertex V1, V2;      /* �����<V1, V2> */
	WeightType Weight;  /* Ȩ�� */
};
typedef PtrToENode Edge;

/* �ڽӵ�Ķ��� */
typedef struct AdjVNode* PtrToAdjVNode;
struct AdjVNode {
	Vertex AdjV;        /* �ڽӵ��±� */
	WeightType Weight;  /* ��Ȩ�� */
	PtrToAdjVNode Next;    /* ָ����һ���ڽӵ��ָ�� */
};

/* �����ͷ���Ķ��� */
typedef struct Vnode {
	PtrToAdjVNode FirstEdge;/* �߱�ͷָ�� */
	DataType Data[100];            /* �涥������� */
	/* ע�⣺�ܶ�����£����������ݣ���ʱData���Բ��ó��� */
} AdjList[MaxVertexNum];    /* AdjList���ڽӱ����� */

/* ͼ���Ķ��� */
typedef struct GNode* PtrToGNode;
struct GNode {
	int Nv;     /* ������ */
	int Ne;     /* ����   */
	AdjList G;  /* �ڽӱ� */
};
typedef PtrToGNode LGraph; /* ���ڽӱ�ʽ�洢��ͼ���� */

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




//��������ģ��->
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
//��������ģ��<-


int main()
{

	const char* filename = "graph_data.txt";
    const char* loadFilename = "graph_data.txt";
	while(1)
	{
		menu();
		int x=0;
		cout << "������������:" << endl;
		cin >> x;
		switch (x)
		{
			case 1:Graph = BuildGraph();
				break;

			case 2:search(Graph); break;

			case 3:system("cls"); menu1(); 
					int button; 
					cout << "�������Ӧ�������:";
					cin >> button;
					if (button == 1) {
						char name[50];
						cout << "��Ҫɾ���Ľڵ�����" << endl;
						cin >> name;
						delete_points(Graph, name);
					}
					else if (button == 2) {
						char name[50];
						char after_name[50];
						cout << "��Ҫ�޸ĵĽڵ�����" << endl;
						cin >> name;
						getchar();
						cout << "�������޸ĺ������" << endl;
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
						cout << "��������Ҫ�޸ıߵ�����Ʒ����:";
						cin >> name1;
						getchar();
						cout << endl;
						cout << "��������Ҫ�޸ıߵ��յ��Ʒ����:";
						cin >> name2;
						getchar();
						cout << endl;
						cout << "�������µ�Ȩ��: ";
						cin >> newWeight;
						start = getPosition(Graph, name1);
						end = getPosition(Graph, name2);
						modifyEdgeWeight(Graph, start, end, newWeight);
					}
					break;
			case 4:
				int s; char name[50];
				cout << "��������ʼ��Ʒ���ƣ�"; 
				cin >> name;
				s = getPosition(Graph, name);
				cal_min_route(Graph,s,prevv,dist); 
				break;
			case 5:
				char need_name1[50],need_name2[50];
				cout << "������Դ���䣬��������ʹ�ù涨��Ʒ������Ŀ���Ʒ" << endl;
				cout << "����������Ʒ���ƣ�"<<endl;
				cin >> need_name1; 
				getchar();
				cout << "������Ŀ���Ʒ���ƣ�"<<endl;
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
				cout << "�����رճ���" << endl;
				goto a;

			default:
				cout << "�����������ȷ������������" << endl;
				system("pause");
				system("cls");
		}
	}
	a:system("pause");
	return 0;
}


//��ѯ��ԭ�����ɵĲ���->
void search(LGraph Graph)
{
	char name[50];
	int save = 0;
	cout << "�������ѯ�˲�Ʒ�����ɲ�Ʒ:" << endl;
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
		cout << "�ò�Ʒ��������Ʒ" << endl;
		return;
	}

	while (p)
	{

		cout << Graph->G[p->AdjV].Data<<" ";
		p = p->Next;
	}
	cout << endl;
}
//��ѯ��ԭ�����ɵĲ���<-

//�������·->
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

	cout << "  �Ӳ�Ʒ" << Graph->G[vs].Data << "��������Ʒ��������Դ���ģ�\n";
	for (int i = 0; i < Graph->Nv; i++) {
		if (strcmp(Graph->G[i].Data, Graph->G[vs].Data) != 0) {
			cout << "   " << Graph->G[vs].Data << "��" << Graph->G[i].Data << "��������Դ����Ϊ��" << endl;
			if (dist[i] != INF) {
				cout << "    " << Graph->G[vs].Data;
				for (int q = MaxVertexNum - 1; q >= 0; q--) {
					if (path[i][q] == -1)
						continue;
					cout << "->" << Graph->G[path[i][q]].Data;
				}
				cout << "->" << Graph->G[i].Data << "����Դ����Ϊ��" << dist[i] << endl;
			}
			else {
				cout << "   ��������ϵ" << endl;
			}
		}
	}
}
//�������·<-

//���ݶ�̬����->
void infor_inserc(LGraph Graph)
{
	int num_points = Graph->Nv;
	int num_edges = Graph->Ne;
	int num_need_points=0;
	int num_need_edges = 0;
	cout << "��Ҫ����Ľڵ����:";
	cin >> num_need_points;
	getchar();
	for (int i = 0; i < num_need_points; i++)
		Graph->G[i + num_points].FirstEdge = NULL;
	Graph->Nv = num_points + num_need_points;
	cout << "Ŀǰϵͳ��ӵ�еĲ�Ʒ����" << Graph->Nv << endl;
	
	
	for (int i = 0; i < num_need_points; i++)
	{
		cout << "�������Ʒ���"<<num_points+i<<"�Ĳ�Ʒ����" << ":" << endl;
		cin >> Graph->G[i+num_points].Data;
		getchar();
	}

	cout << "��Ҫ����ı�����";
	cin >> num_need_edges;
	for (int i = 0; i < num_need_edges; i++)
	{
		char A[50], B[50];
		PtrToENode p = new ENode;
		cout << "����������㡢�յ㡢Ȩ�أ�";
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
//���ݶ�̬����<-


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


//����ɾ��-��
void delete_points(LGraph Graph, char name[50])
{
    int k = -1, cnt = 0;

    // ���Ҵ�ɾ���ڵ��λ��
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
        cout << "δ�ҵ�Ҫɾ���Ĳ�Ʒ��" << name << endl;
        return;
    }

    // ɾ����ýڵ���صı�
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

    // ɾ���ýڵ�
    for (int i = k + 1; i < Graph->Nv; i++)
    {
        Graph->G[i - 1] = Graph->G[i];

        // ���������ڵ���ڽӱ��еĽڵ���
        PtrToAdjVNode p = Graph->G[i - 1].FirstEdge;
        while (p)
        {
            if (p->AdjV > k)
                p->AdjV--;
            p = p->Next;
        }
    }

    // ��������
    Graph->Ne -= cnt;

    Graph->Nv--;
    cout << "��Ʒ " << name << " �ѳɹ�ɾ����" << endl;
}

//����ɾ����-


void two_point_route(LGraph Graph, char name1[50], char name2[50], int prevv[], int dist[])
{
	int x = getPosition(Graph, name1);
	int y = getPosition(Graph, name2);
	if (x == -1 || y == -1)
	{
		cout << "������Ĳ�Ʒ���Ʋ���ϵͳ�У����ѯ������������ȷ��Ʒ����" << endl;
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
			cout << "~~~~~" << Graph->G[x].Data << "��" << Graph->G[i].Data << "����Դ����Ϊ��" << endl;
			if (dist[i] != INF) {
				cout << "    " << Graph->G[x].Data;
				for (int q = MaxVertexNum - 1; q >= 0; q--) {
					if (path[i][q] == -1)
						continue;
					cout << "->" << Graph->G[path[i][q]].Data;
				}
				cout << "->" << Graph->G[i].Data << "����Դ����Ϊ��" << dist[i] << endl;
			}
			else {
				cout << "   �޷�����" << endl;
			}
		}
	}
	cout << "�밴����·�ߵ�����Դ�����ϵ,�Ա�֤������Դ����" << endl;
}


void change_data(LGraph Graph, char name1[], char name2[])
{
	int l = getPosition(Graph, name1);
	if (l == -1)
	{
		cout << "������Ĳ�Ʒ���Ʋ���ϵͳ�У����ѯ������������ȷ��Ʒ����" << endl;
		return;
	}
	cout << "��Ʒ���:" << l<<endl;
	strcpy(Graph->G[l].Data, name2);
	cout << "�����޸Ĳ�Ʒ�������" << endl;
	cout << Graph->G[l].Data << endl;
}

// �޸ıߵ�Ȩ��
void modifyEdgeWeight(LGraph Graph, Vertex start, Vertex end, WeightType newWeight) {
	// �ҵ���� start ��Ӧ������ͷ
	PtrToAdjVNode p = Graph->G[start].FirstEdge;

	// ���������ҵ��յ�Ϊ end �ı�
	while (p != NULL && p->AdjV != end) {
		p = p->Next;
	}

	// ����ҵ��˶�Ӧ�ıߣ��޸�Ȩ��
	if (p != NULL) {
		p->Weight = newWeight;
		cout << "�ɹ��޸�������ϵ (" << Graph->G[start].Data << ", " << Graph->G[end].Data << ") ����Դ����Ϊ " << newWeight << endl;
	}
	else {
		cout << "δ�ҵ���ز�Ʒ������ϵ (" << Graph->G[start].Data << ", " << Graph->G[end].Data << ")���޷��޸���Դ����" << endl;
	}
}



LGraph CreateGraph(int VertexNum)
{ /* ��ʼ��һ����VertexNum�����㵫û�бߵ�ͼ */
	Vertex V;
	LGraph Graph;

	Graph = (LGraph)malloc(sizeof(struct GNode)); /* ����ͼ */
	Graph->Nv = VertexNum;
	Graph->Ne = 0;
	/* ��ʼ���ڽӱ�ͷָ�� */
	/* ע�⣺����Ĭ�϶����Ŵ�0��ʼ����(Graph->Nv - 1) */
	for (V = 0; V < Graph->Nv; V++)
	{
		Graph->G[V].FirstEdge = NULL;
	}

	return Graph;
}

void InsertEdge(LGraph Graph, Edge E)
{
	PtrToAdjVNode NewNode;

	/* ����� <V1, V2> */
	/* ΪV2�����µ��ڽӵ� */
	NewNode = (PtrToAdjVNode)malloc(sizeof(struct AdjVNode));
	NewNode->AdjV = E->V2;
	NewNode->Weight = E->Weight;
	/* ��V2����V1�ı�ͷ */
	NewNode->Next = Graph->G[E->V1].FirstEdge;
	Graph->G[E->V1].FirstEdge = NewNode;
}

LGraph BuildGraph()
{
	LGraph Graph;
	Edge E;
	Vertex V;
	int Nv, i;
	cout << "�����붥�����:";
	cin >> Nv;  /* ���붥����� */
	Graph = CreateGraph(Nv); /* ��ʼ����Nv�����㵫û�бߵ�ͼ */

	getchar();
	cout << "�������Ʒ���ƣ�" << endl;
	/* ������������ݵĻ����������� */
	for (V = 0; V < Graph->Nv; V++)
	{
		cout << "�������Ʒ���Ϊ" << V << "�Ĳ�Ʒ����:";
		cin >> Graph->G[V].Data;
		getchar();
	}

	cout << "�����������";
	cin >> Graph->Ne;
	// scanf("%d", &(Graph->Ne));   /* ������� */
	if (Graph->Ne != 0) { /* ����б� */
		E = (Edge)malloc(sizeof(struct ENode)); /* �����߽�� */
		/* ����ߣ���ʽΪ"��� �յ� Ȩ��"�������ڽӾ��� */
		for (i = 0; i < Graph->Ne; i++) {
			char A[50], B[50];
			cout << "������������Ʒ���ơ��յ��Ʒ���ơ�Ȩ�أ�"<<endl;
			cin >> A; getchar(); cin >> B; getchar(); cin >> E->Weight;
			E->V1 = getPosition(Graph, A);
			E->V2 = getPosition(Graph, B);
			 /* ע�⣺���Ȩ�ز������ͣ�Weight�Ķ����ʽҪ�� */
			InsertEdge(Graph, E);
		}
	}
	return Graph;
}

void menu()
{
	cout << "------------------------" << endl;
	cout << "*   �ǻ���Դ����ϵͳ   *" << endl;
	cout << "------------------------" << endl;
	cout << "*      1.��ʼ��Դ����  *" << endl;
	cout << "*      2.��Դ���ݲ�ѯ  *" << endl;
	cout << "*      3.��̬���ݲ���  *" << endl;
	cout << "*      4.������Դ�ṹ  *" << endl;
	cout << "*      5.������Դ����  *" << endl;
	cout << "*      6.�����ļ�      *" << endl;
	cout << "*      7.��ȡ�ļ�      *" << endl;
	cout << "*      0.�رճ���      *" << endl;
	cout << "*----------------------*" << endl;
}

void menu1()
{
	cout << "------------------------" << endl;
	cout << "|      ��̬���ݲ���    |" << endl;
	cout << "------------------------" << endl;
	cout << "|      1.����ɾ��      |" << endl;
	cout << "|      2.�����޸�      |" << endl;
	cout << "|      3.�������      |" << endl;
	cout << "|      4.��Ȩ�޸�      |" << endl;
	cout << "|----------------------|" << endl;
}

void saveToFile(LGraph Graph, const char* filename) {
    std::ofstream outFile(filename);

    if (!outFile.is_open()) {
        std::cerr << "�޷����ļ���" << filename << std::endl;
        return;
    }
	cout<<"��Ʒ��:"<<Graph->Ne<<"��Ʒ��ϵ��:"<<Graph->Nv<<endl;
    // ���涥����Ϣ
    outFile << Graph->Nv << std::endl;
    for (int i = 0; i < Graph->Nv; i++) {
        outFile << Graph->G[i].Data << std::endl;
    }

    // �������Ϣ
    outFile << Graph->Ne << std::endl;
    for (int i = 0; i < Graph->Nv; i++) {
        PtrToAdjVNode p = Graph->G[i].FirstEdge;
        while (p) {
            outFile << Graph->G[i].Data << " " << Graph->G[p->AdjV].Data << " " << p->Weight << std::endl;
            p = p->Next;
        }
    }

    outFile.close();
    std::cout << "ͼ�����ѱ��浽�ļ���" << filename << std::endl;
}

// ���ļ���ȡͼ����
LGraph readFromFile(const char* filename) {
    std::ifstream inFile(filename);

    if (!inFile.is_open()) {
        std::cerr << "�޷����ļ���" << filename << std::endl;
        return nullptr;
    }
	
    int Nv, Ne;
    inFile >> Nv; // ��ȡ������
    LGraph Graph = CreateGraph(Nv);

    // ��ȡ������Ϣ
    for (int i = 0; i < Nv; i++) {
        inFile >> Graph->G[i].Data;
    }

    // ��ȡ����Ϣ
    inFile >> Ne; // ��ȡ����
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
    std::cout << "ͼ�����Ѵ��ļ����أ�" << filename << std::endl;

    return Graph;
}
