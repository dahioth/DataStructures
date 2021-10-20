/*

  AUTHORS : Dahi Othmane & Daima Riad
  Instructor: R. Abid
  TOPIC : Graphs
  
        -- Project#3 --
          Spring 2021

*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define ID_SIZE 10
#define MAX_N 200
#define INF 1000000007

typedef struct v {
	char ID[ID_SIZE];
	double x, y;
	struct v *next;
} vertex;

typedef struct {
  char ID1[ID_SIZE], ID2[ID_SIZE];
  double distance;
} edge;

vertex *adj_list[MAX_N];
int N;
double range;

char visited_vehicles[MAX_N][ID_SIZE], visited_vehicles2[MAX_N][ID_SIZE];
int size_ = 0, size_2 = 0;

// Shortest Path Result variables
char result[MAX_N][ID_SIZE];
int size_res = 0;

/* PROTOTYPES */
void read_data();
void build_adj_list();
void menu();
void v_insert(vertex *, vertex *);
double get_distance(vertex *, vertex *);
void display_adj(vertex *);
int is_adj(vertex *, vertex *);
vertex *search_v(char*);
int search_index_v(char*);
void add_v(char*, double, double);
void delete_v(char*);
void delete_edge(char*, char*);
void move_v(char*, double, double);
int is_visited(char*);
void DFSearch(char*);
void BFSearch(vertex* root);
void enqueue_v(char tbvis[MAX_N][ID_SIZE], int* top, char* ID);
void dequeue_v(int* top, int* rear);
int isFull(int top);
int isEmpty(int top, int rear);
void MST(vertex*);
void sort(edge arr[], int sz);
void swap_edge(edge* e1, edge* e2);
void SPath(vertex*, vertex*);
int is_reachable(char* ,char*);
void DFS_(char* ID);
int is_visited2(char* ID);
/* PROTOTYPES */

int main() {
  read_data();
  build_adj_list();
  menu();
}

void read_data() {

  FILE *fin = fopen("data.txt", "r");
  if (fin != NULL) {

    fscanf(fin, "%d", &N);
    fscanf(fin, "%lf", &range);

    for (int i = 0; i < N; i++) {
      adj_list[i] = (vertex *)malloc(sizeof(vertex));
      fscanf(fin, "%s", adj_list[i]->ID);
      fscanf(fin, "%lf", &adj_list[i]->x);
      fscanf(fin, "%lf", &adj_list[i]->y);
      adj_list[i]->next = NULL;
    }

    fclose(fin);
  } else {
    printf("No such file found.");
  }
}

double get_distance(vertex *vi, vertex *vj) {
  return sqrt(pow((vi->x - vj->x), 2) + pow((vi->y - vj->y), 2));
}

int is_adj(vertex *a, vertex *b) {
  return (get_distance(a, b) <= range && get_distance(a, b) > 0);
}

void build_adj_list() {
  vertex *new_vehicle;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (i == j)
        continue;

      if (is_adj(adj_list[i], adj_list[j])) {
        new_vehicle = (vertex *)malloc(sizeof(vertex));
        strcpy(new_vehicle->ID, adj_list[j]->ID);
        new_vehicle->x = adj_list[j]->x;
        new_vehicle->y = adj_list[j]->y;
        new_vehicle->next = NULL;

        v_insert(adj_list[i], new_vehicle);
      }
    }
  }
}

void v_insert(vertex *v, vertex *adj_v) {
  double distance = get_distance(v, adj_v);
  vertex *w;
  if (v->next == NULL)
    v->next = adj_v;

  else {
    if (distance <= get_distance(v, v->next)) {
      adj_v->next = v->next;
      v->next = adj_v;
      return;
    }
    for (w = v->next; w->next != NULL; w = w->next) {
      if (distance <= get_distance(v, w->next)) {
        adj_v->next = w->next;
        w->next = adj_v;
        return;
      }
    }
    w->next = adj_v;
  }
}

/* MENU */
void menu() {
    int answer;
    char ID[ID_SIZE];

    char ID1[ID_SIZE];
    char ID2[ID_SIZE];
    double x, y;

    double sum_distance = 0;

    do {
        printf("\n\n**********************************\n*************  Menu  *************\n**********************************\n");
        printf("\n1. Display All Edges.\n2. Display Adjacent Vehicle.\n3. Add a Vehicle.\n4. Delete a Vehicle.\n5. Delete an Edge.\n6. Move a Vehicle.\n7. DFSearch.\n8. BFSearch.\n9. MST (Prim).\n10. Shortest Path.\n11. Quit.\n");
        printf("\n**********************************\n************ END Menu ************\n**********************************\n");

        printf("Your choice: ");
        scanf("%d", &answer);
        getchar();

        switch (answer) {
        case 1:
          for (int i = 0; i < N; i++)
            display_adj(adj_list[i]);
          break;

        case 2:
          printf("Please enter the ID of the vehicle: ");
          scanf("%s", ID);

          if (!search_v(ID))
              printf("No such vehicle.\n");
          else
              display_adj(search_v(ID));
          break;

        case 3:
          printf("Please enter the ID of the new vehicle: "); scanf("%s", ID);
          printf("Please enter the x y coordinates: "); scanf("%lf %lf", &x, &y);
          add_v(ID, x, y);
          break;

        case 4:
          printf("Please enter the ID of the vehicle: "); scanf("%s", ID);
          if (search_index_v(ID) < 0)
            printf("No such vehicle.\n");
          else
            delete_v(ID);
          break;

        case 5:
          printf("Please enter the ID of the two vehicle: "); scanf("%s %s", ID1, ID2);
          if (search_index_v(ID1) < 0 || search_index_v(ID2) < 0)
            printf("No such vehicles.\n");
          else
            if (is_adj(search_v(ID1), search_v(ID2)))
              delete_edge(ID1, ID2);
            else
              printf("Vehicles not adjacent.\n");
          break;
        
        case 6:
          printf("Please enter the ID of the vehicle: "); scanf("%s", ID);
          if (search_v(ID)){
						printf("Please enter the new x y coordinates: "); scanf("%lf %lf", &x, &y);
						move_v(ID, x, y);
					}
          else
            printf("No such vehicle.\n");
          break;
        case 7:
          printf("Please enter the ID from where you want to start the DFS: ");
          scanf("%s", ID);
          if (!search_v(ID))
            printf("No such vehicle.\n");
          else {
            DFSearch(ID);
            for (int i = 0; i < size_; i++)
              strcpy(visited_vehicles[i], "NULL");
            size_ = 0;
          }
          break;
				
				case 8:
          printf("Please enter the ID from where you want to start the BFS: ");
          scanf("%s", ID);
          if (!search_v(ID))
            printf("No such vehicle.\n");
          else {
            BFSearch(search_v(ID));
            // for (int i = 0; i < size_; i++) => No need, to empty size_ = 0 is sufficient;
            // strcpy(visited_vehicles[i], "NULL");
            size_ = 0;
          }
          //(search_v(ID2)==NULL) ? printf("No such vehicle.\n") : BFSearch(search_v(ID2));
					break;

				case 9:
          printf("Please enter the ID from where you want to start the MST: ");
          scanf("%s", ID);
          if (!search_v(ID))
            printf("No such vehicle.\n");
          else {
            MST(search_v(ID));
            size_ = 0;
          }
					break;

				case 10:
          printf("Please enter the ID of the two vehicle: "); scanf("%s %s", ID1, ID2);
          if (search_index_v(ID1) < 0 || search_index_v(ID2) < 0)
            printf("No such vehicles.\n");
					else if (!strcmp(ID1,ID2)) {
            printf("Same vehicle.\n");
					}
					else if(!is_reachable(ID1, ID2)) {
						printf("No path found.\n");
					}
          else {

            SPath(search_v(ID1), search_v(ID2));

            for (int i = size_res - 1; i >= 0; i--)
              printf("%s%s", result[i], i == 0 || i == size_res ? "" : " -> ");
            for (int i = size_res - 1; i > 0; i--)
              sum_distance += get_distance(search_v(result[i]), search_v(result[i-1]));
            printf("\nMinimum distance: %.2f\n", sum_distance);

            // Empty everything
            size_res = 0;
            size_ = 0;
            sum_distance = 0;
						size_2 = 0;
          }
					break;

        case 11:
          printf("Thank you for using our program.\nGood bye.\n");
          break;

        default:
          printf("Wrong input.\nPlease try again.\n");
          break;
        }
    } while (answer != 11);
}

void display_adj(vertex *root) {
  if (root == NULL)
    return;
  for (vertex *w = root->next; w != NULL; w = w->next) {
    printf("%s %s %.2f\n", root->ID, w->ID, get_distance(root, w));
  }
}

vertex *search_v(char *ID) {
  for (int i = 0; i < N; i++)
    if (!strcmp(adj_list[i]->ID, ID))
      return adj_list[i];
  return NULL;
}

int search_index_v(char* ID) {
  for (int i = 0; i < N; i++) {
    if (!strcmp(adj_list[i]->ID, ID))
      return i;
  }
  return -1;
}

void add_v(char* ID, double x, double y) {
	vertex *new_vehicle;
  adj_list[N] = (vertex *)malloc(sizeof(vertex));
  strcpy(adj_list[N]->ID, ID);
  adj_list[N]->x = x;
  adj_list[N]->y = y;
  adj_list[N]->next = NULL;
  for (int i = 0; i < N-1; i++) {
    if (is_adj(adj_list[i], adj_list[N])) {
			new_vehicle = (vertex *)malloc(sizeof(vertex));
			strcpy(new_vehicle->ID, adj_list[i]->ID);
			new_vehicle->x = adj_list[i]->x;
			new_vehicle->y = adj_list[i]->y;
			new_vehicle->next = NULL;
			v_insert(adj_list[N], new_vehicle);

			new_vehicle = (vertex *)malloc(sizeof(vertex));
			strcpy(new_vehicle->ID, adj_list[N]->ID);
			new_vehicle->x = adj_list[N]->x;
			new_vehicle->y = adj_list[N]->y;
			new_vehicle->next = NULL;
			v_insert(adj_list[i], new_vehicle);

    }
  }
  N++;
}

void delete_v(char* ID) {
  int index_v = search_index_v(ID);
  for (vertex* w = adj_list[index_v]; w != NULL; w = w->next) {
    delete_edge(w->ID, ID);
  }
  if (index_v < N-1) {
    for (int i = index_v; i < N; i++)
      adj_list[i] = adj_list[i+1];
    N--;
  } else 
    N--;
}

void delete_edge(char* ID1, char* ID2) {
  vertex* v2 = search_v(ID1);

  if (v2->next == NULL)
    return;

  vertex* w;
  for (w = v2; w->next != NULL; w = w->next) {
    if (!strcmp(w->next->ID, ID2)) {
      w->next = w->next->next;
      delete_edge(ID2, ID1);
      return;
    }
  }
}
void move_v(char* ID, double x, double y) {
  // get index of the vehicle one we want to move
  int v_index = search_index_v(ID);
  // we empty it's adj list;
  for (vertex* w = adj_list[v_index]; w != NULL; w = w->next) {
    delete_edge(w->ID, ID);
  }
  // we set him to NULL to keep the position 'v_index' filled
  adj_list[v_index]->next = NULL;
  // we replace the NULL on the position i with the new informations (in arguments)
  adj_list[v_index] = (vertex *)malloc(sizeof(vertex));
  strcpy(adj_list[v_index]->ID, ID);
  adj_list[v_index]->x = x;
  adj_list[v_index]->y = y;
  adj_list[v_index]->next = NULL;

  /* ADD PART */
  // we fill his adj list with every vehicle that is adj to him
  vertex* new_vehicle;
  for (int i = 0; i < N; i++) {
    if (i != v_index) {
      if (is_adj(adj_list[i], adj_list[v_index])) {
        new_vehicle = (vertex *)malloc(sizeof(vertex));
        strcpy(new_vehicle->ID, adj_list[i]->ID);
        new_vehicle->x = adj_list[i]->x;
        new_vehicle->y = adj_list[i]->y;
        new_vehicle->next = NULL;
        v_insert(adj_list[v_index], new_vehicle);
      }
    }
  }
  // we iterate over the adj list of the vehicle to add him to the adj list vehicle
  for (vertex* w = adj_list[v_index]; w->next != NULL; w = w->next) {
    new_vehicle = (vertex *)malloc(sizeof(vertex));
    strcpy(new_vehicle->ID, adj_list[v_index]->ID);
    new_vehicle->x = adj_list[v_index]->x;
    new_vehicle->y = adj_list[v_index]->y;
    new_vehicle->next = NULL;
    v_insert(search_v(w->next->ID), new_vehicle);
  }
  /* ADD PART */
}

void DFSearch(char* ID) {
	
  vertex* w;
  strcpy(visited_vehicles[size_++], ID);
  printf("%s ", ID);
  for (w = search_v(ID); w; w = w->next)
    if (!is_visited(w->ID))
      DFSearch(w->ID);
}

void BFSearch(vertex* root) {
	size_=0;

	if(root==NULL) return;

	char to_be_visited[MAX_N][ID_SIZE];
	int top=-1, rear=-1;

	enqueue_v(to_be_visited, &top, root->ID);

	while(!isEmpty(top, rear)) {
		if(!is_visited(to_be_visited[rear+1])) {
			for(vertex* w=search_v(to_be_visited[rear+1]); w!=NULL; w=w->next) {
				if (!is_visited(w->ID)) {
					enqueue_v(to_be_visited, &top, w->ID);
					//strcpy(visited_vehicles[size_++], w->ID);
				}
			}

			strcpy(visited_vehicles[size_++], to_be_visited[rear+1]);
			printf("%s ", to_be_visited[rear+1]);
			dequeue_v(&top, &rear);
		}
		else dequeue_v(&top, &rear);
	}
}

int is_visited(char* ID) {
	for (int i = 0; i < size_; i++) {
		if (!strcmp(ID, visited_vehicles[i])) return 1;
	}
	return 0;
}

void enqueue_v(char tbvis[MAX_N][ID_SIZE], int* top, char* ID) {
	if (!isFull(*top)) {
		strcpy(tbvis[++(*top)], ID);
	}

}

void dequeue_v(int* top, int* rear) {
	if (!isEmpty(*top, *rear)) (*rear)++;
}

int isFull(int top) {
	return (top==MAX_N-1);
}

int isEmpty(int top, int rear) {
	return (top==rear && top!=-1);
}

int is_MSTedge(vertex* a, vertex* b) {
	return ((is_visited(a->ID) && !is_visited(b->ID)) || (!is_visited(a->ID) && is_visited(b->ID))) && is_adj(a, b);
}

void strswap(char a[], char b[]) {
  char temp[ID_SIZE];
  strcpy(temp, a);
  strcpy(a, b);
  strcpy(b, temp);
}

void MST(vertex* root) {
	edge mst[MAX_N];
  int MST_size = 0;
  edge min;
  strcpy(visited_vehicles[size_++], root->ID);
  while (MST_size < N-1) {
    strcpy(min.ID1, "ID-1");
    strcpy(min.ID2, "ID-2");
    min.distance = range + 1;
    for (int i = 0; i < size_; i++) {
      for (vertex* w = search_v(visited_vehicles[i]); w; w = w->next) {
        if (is_MSTedge(search_v(visited_vehicles[i]), w)) {
          if (get_distance(search_v(visited_vehicles[i]), w) < min.distance) {
            strcpy(min.ID1, visited_vehicles[i]);
            strcpy(min.ID2, w->ID);
            min.distance = get_distance(search_v(visited_vehicles[i]), w);
          }
        }
      }
    }
    if (min.distance == range + 1)
      break;
    if (is_visited(min.ID1))
      strcpy(visited_vehicles[size_++], min.ID2);
    else {
      strcpy(visited_vehicles[size_++], min.ID1);
      strswap(min.ID1, min.ID2);
    }
    mst[MST_size++] = min;
  }
  if (MST_size < N-1) {
  	printf("No Spanning Tree\n");
		return;
	}
	sort(mst, MST_size);
	for (int i=0; i<MST_size; i++) printf("(%s, %s, %.2lf)\n", mst[i].ID1, mst[i].ID2, mst[i].distance);
}

void sort(edge arr[], int sz) {
	for (int i=0; i<sz; i++) {
		for (int j=0; j < sz - 1 - i; j++)
			if (arr[j].distance > arr[j+1].distance) swap_edge(&arr[j], &arr[j+1]);
	}
}

void swap_edge(edge* e1, edge* e2) {
	edge temp = *e1;
	*e1 = *e2;
	*e2 = temp;
}

void SPath(vertex* from, vertex* to) {
  // INIT //
  double shortest_distances_from[MAX_N];
  char prev_vertex[MAX_N][ID_SIZE];
  for (int i = 0; i < N; i++)
    shortest_distances_from[i] = INF;
  shortest_distances_from[search_index_v(from->ID)] = 0;
  // INIT//

  // Iteration //
  while (size_ < N) {
    double min_ = INF;
    int min_index = 0;
    char to_be_checked[ID_SIZE];
    for (int i = 0; i < N; i++) {
      if (!is_visited(adj_list[i]->ID)) {
        if (shortest_distances_from[i] < min_) {
          min_ = shortest_distances_from[i];
          min_index = i;
        }
      }
    }
    strcpy(to_be_checked, adj_list[min_index]->ID);

    for (vertex* w = search_v(to_be_checked)->next; w; w = w->next) {
      if (!is_visited(w->ID)) {
        if (shortest_distances_from[min_index] + get_distance(search_v(to_be_checked), w) < shortest_distances_from[search_index_v(w->ID)]) {
          shortest_distances_from[search_index_v(w->ID)] = shortest_distances_from[min_index] + get_distance(search_v(to_be_checked), w);
          strcpy(prev_vertex[search_index_v(w->ID)], to_be_checked);
        }
      }
    }
    strcpy(visited_vehicles[size_++], to_be_checked);
  }
  // end of Iteration //
  strcpy(result[size_res++], to->ID);

  char to_give[ID_SIZE];
  strcpy(to_give, prev_vertex[search_index_v(to->ID)]);
  while (strcmp(to_give, from->ID)) {
    strcpy(result[size_res++], to_give);
    strcpy(to_give, prev_vertex[search_index_v(to_give)]);
  }
  strcpy(result[size_res++], from->ID);
}

int is_reachable(char* ID1, char* ID2) {
	int flag=0;
	DFS_(ID1);

	for (int i=0; i<size_2; i++) {
		if (!strcmp(visited_vehicles2[i], ID2)) flag=1;
	}
	return flag==1;
}

void DFS_(char* ID) {
	
  vertex* w;
  strcpy(visited_vehicles2[size_2++], ID);
  for (w = search_v(ID); w; w = w->next)
    if (!is_visited2(w->ID))
      DFS_(w->ID);
}

int is_visited2(char* ID) {
	for (int i = 0; i < size_2; i++) {
		if (!strcmp(ID, visited_vehicles2[i])) return 1;
	}
	return 0;
}