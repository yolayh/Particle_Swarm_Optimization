#include <iostream>
#include <cmath>
#include <vector>
#include <time.h>

using namespace std;

typedef vector<double> vec;


//變數
vector<vec> solution;
vector<vec> velocity;                    
vector<vec> individual_best;
vec group_best_position;
double best_result;

//function
double evaluation(vec);
void initialization(int, int);
void update_position(double, double, double);
bool check_bound(double);
void update_individual();
void find_best(vector<vec>);
void execution(int, int, int, double, double, double);
void print_vec_group(vector<vec>);

//Ackley function 
//  (2維: 變數為x跟y)
//  -20.0 * exp(-0.2 * sqrt(0.5 * (x**2 + y**2)))-
//  exp(0.5 * (cos(2 * pi * x)+cos(2 * pi * y)))  +
//  exp(1) +
//  20

//r_min, r_max = -32.768, 32.768

// f(x)= -20*exp(-0.2*sqrt(1/d)sum(pow(xi,2))) - exp(1/d*cos(2*pi*xi)) + exp(1) + 20


double evaluation(vec a){
    double value = 0;
    double quadratic = 0;
    double cos_sum = 0;
    int size = a.size();

    for(int i = 0; i < size; i++){
        quadratic += pow(a[i], 2);
    }

    for(int i = 0; i < size; i++){
        cos_sum += cos(2*M_PI*a[i]);
    }

    value = - (20*exp(-0.2*sqrt((1.0/size)*quadratic))) - (exp ((1.0/size)*cos_sum)) + exp(1) + 20;

    return value;
}


void initialization(int dimension, int pop_size){
    double max = 32.768;
    double min = -32.768;
    vec v;

    //設定初始值(position)
    for(int k =0; k < pop_size; k++){
        v.clear();
        for(int i =0; i<dimension; i++){
            double rand_num = (max - min) * (rand() / (RAND_MAX + 1.0)) + min;
            v.push_back(rand_num);
        }
        solution.push_back(v);
    }

    //加速度初始值 #設定為0
    vec zero(dimension, 0.0);
    for(int k =0; k < pop_size; k++){
        velocity.push_back(zero);
    }
    
    //初始值設定為個體最佳解
    individual_best = solution;
    for(int k =0; k < pop_size; k++){
        double now_value = evaluation(solution[k]);
        individual_best[k].push_back(now_value);
    }

    //群體最佳解
    best_result = individual_best[0][dimension];
    group_best_position = solution[0];
    for(int i = 1; i < pop_size; i++){
        if(individual_best[i][dimension] < best_result){
            best_result = individual_best[i][dimension];
            group_best_position = individual_best[i];
        }
    }

}

//transition
void update_position(double inertia, double p_coefficient, double g_coefficient){
    int pop_size = solution.size();
    int dimension = solution[0].size();

    for (int i =0; i < pop_size; i++){
        double rand_1 = ((double) rand() / (RAND_MAX));
        double rand_2 = ((double) rand() / (RAND_MAX));
        for (int k =0; k < dimension; k++){
            double new_v = inertia*velocity[i][k] + rand_1 * p_coefficient * (individual_best[i][k]-solution[i][k]) +
                                rand_2 * g_coefficient * (group_best_position[k] - solution[i][k]);
            velocity[i][k] = new_v;
            solution[i][k] += new_v;

            if (check_bound(solution[i][k])){
                if (solution[i][k] > 32.768){
                    solution[i][k] = 32.768;
                }
                else if (solution[i][k] < -32.768){
                    solution[i][k] = -32.768;
                }
            }
            
        }
        
    }
}


//check position is in the boundary or not
bool check_bound(double num){
    bool leave_boundary = false;
    double max = 32.768;
    double min = -32.768;
    if (num > max or num < min){
        return true;
    }

    return false;
}

//更新individual best solution
void update_individual(){
    int pop_size = solution.size();
    int dimension = solution[0].size();

    for(int i =0; i < pop_size; i++){
        if(evaluation(solution[i]) < individual_best[i][dimension]){
            for(int k =0; k <dimension; k++){
                individual_best[i][k] = solution[i][k];
            }
            individual_best[i][dimension] = evaluation(solution[i]);
        }
    }
}


//從individual best solution 找最佳解
void find_best(vector<vec> p_best_solution){
    int pop_size = solution.size();
    int dimension = solution[0].size();
    for(int i =0; i < pop_size; i++){
        if ( p_best_solution[i][dimension] < best_result){
            best_result = p_best_solution[i][dimension];
            group_best_position = solution[i];
        }
    }
}

void execution(int iteration, int dimension, int pop_size, double inertia, double p_coefficient, double g_coefficient){
    
    srand( time(NULL) );
    initialization(dimension, pop_size);
    
    for(int i =0; i<iteration; i++){
        cout << "------iteration-" << i << endl;
        update_position(inertia, p_coefficient, g_coefficient);
        update_individual();
        find_best(individual_best);
        
        print_vec_group(individual_best);
    }

    cout << endl;
    cout << "best answer:" << endl;
    
    for(auto y:group_best_position){
        cout << y << " ";
    }

    cout << "\n" << best_result << endl;
}

void print_vec_group(vector<vec> v){
    for (int i = 0; i<v.size(); i++){
        for (int k = 0; k <v[i].size(); k++){
            cout << v[i][k] << " ";
        }
        cout << endl;
    }
}


