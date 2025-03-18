#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

//NNR Time baby

//Structure for a 2D point
typedef struct {
    float x, y;
} Point;

//Structure for a kd tree node
typedef struct KDNode {
    Point point;
    struct KDNode *left, *right;
} KDNode;

//Create a new kd node
KDNode* createNode(Point point) {
    KDNode* node = (KDNode*)malloc(sizeof(KDNode));
    node->point = point;
    node->left = node->right = NULL;
    return node;
}

//Recursive. Insert new point into the kd tree 
KDNode* insert(KDNode* root, Point point, int depth) {
    if (root == NULL)
        return createNode(point);
    
    int axis = depth % 2;
    if ((axis == 0 && point.x < root->point.x) || (axis == 1 && point.y < root->point.y))
        root->left = insert(root->left, point, depth + 1);
    else
        root->right = insert(root->right, point, depth + 1);
    
    return root;
}

//Compute squared Euclidean distance
float distanceSquared(Point a, Point b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

//NNS
void nearestNeighbor(KDNode* root, Point target, int depth, KDNode** best, float* bestDist) {
    if (root == NULL)
        return;
    
    float d = distanceSquared(target, root->point);
    if (d < *bestDist) {
        *bestDist = d;
        *best = root;
    }
    
    int axis = depth % 2;
    KDNode* next = (axis == 0 && target.x < root->point.x) || (axis == 1 && target.y < root->point.y) ? root->left : root->right;
    KDNode* other = (next == root->left) ? root->right : root->left;
    
    nearestNeighbor(next, target, depth + 1, best, bestDist);
    
    if (*bestDist > (axis == 0 ? (target.x - root->point.x) * (target.x - root->point.x) : (target.y - root->point.y) * (target.y - root->point.y))) {
        nearestNeighbor(other, target, depth + 1, best, bestDist);
    }
}

int main() {
    //Point points[] = {{5, 3}, {5, 4}, {5, 6}, {5, 7}, {5, 1}};
    //Point points[] = {{2, 4}, {5, 4}, {9, 4}, {4, 4}, {8, 4}};
    //Point points[] = {{0, 0}, {0,0}, {0, 0}, {0, 0}, {0, 0}};
    //Point points[] = {{99999999, 999999999}, {99999999, 999999999}, {0, 0.000002}, {0, 0.0000001}, {0, 0.0000001}};
    Point points[] = {{2, 3}, {5, 4}, {9, 6}, {4, 7}, {8, 1}};
    int n = sizeof(points) / sizeof(points[0]);
    
    KDNode* root = NULL;
    for (int i = 0; i < n; i++)
        root = insert(root, points[i], 0);
    
    Point target = {6, 5};
    KDNode* best = NULL;
    float bestDist = FLT_MAX;
    
    nearestNeighbor(root, target, 0, &best, &bestDist);
    
    if (best)
        printf("Nearest point to (%.1f, %.1f) is (%.1f, %.1f)\n", target.x, target.y, best->point.x, best->point.y);
    
    return 0;
}
