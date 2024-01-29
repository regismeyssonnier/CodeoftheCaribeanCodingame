#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include <map>
 #include <random>
#include <chrono>
#include <deque>
#include <map>
using namespace std::chrono;
using namespace std;

/**
 * Save humans, destroy zombies!
 **/
class Sim {
public:
    int id;
    int x;
    int y;
    int fire;
    int mine;
    int score;
    int orientation;
    int speed;
    int rhum;
    int id_ship_canon;
    int nb_turn_bimpact;
    int starboard;
    int port;
    int firer = 0;
    int starbr = 0;
    int portr = 0;
    int fireen = 0;
    int ismine = 0;
    int faster;
    int slower;
    int cfire;
    int ind_shoot=0;
    int XE;
    int YE;
    int shoot_mine = 0;
    int ind_mine = 0;
    int move=0;
    int moveb = 0;
    int xb;
    int yb;
    int distb=0;
    
    bool operator<(const Sim& other) const {
        return id < other.id;
    }

};

struct Cube{
    int q;
    int r;
    int s;
    Cube(){}
    Cube(int q, int r, int s):q(q), r(r),s(s){}
};

int whereisdude(int x, int y, int x2, int y2){
    return sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2));
}

Sim cube_to_oddr(Cube hex){
    Sim res;
    if(hex.r %2 == 0)
        res.x = hex.q + (hex.r + (hex.r&1)) / 2;
    else
         res.x = hex.q + (hex.r - (hex.r&1)) / 2;
    res.y = hex.r;
    return res;

}

Cube oddr_to_cube(Sim hex){
    Cube c;
    if(hex.y %2 == 0)
        c.q = hex.x - (hex.y + (hex.y&1)) / 2;
    else
        c.q = hex.x - (hex.y - (hex.y&1)) / 2;
    c.r = hex.y;
    c.s = -c.q-c.r;

    return c;
}

Cube cube_subtract(Cube a, Cube b){
    return Cube(a.q - b.q, a.r - b.r, a.s - b.s);

}

double cube_distance(Cube a, Cube b){
    Cube vec = cube_subtract(a, b);
    return (abs(vec.q) + abs(vec.r) + abs(vec.s)) / 2.0;
}

class Zennemy{
public:
    int id;
    int x;
    int y;
    int nx;
    int ny;

    Zennemy(){}
    Zennemy(int id, int x, int y, int nx, int ny):id(id), x(x), y(y), nx(nx), ny(ny){


    }


};

class Human{
public:
    int id;
    int x;
    int y;
  

    Human(){}
    Human(int id, int x, int y):id(id), x(x), y(y){


    }


};

 class Zombie{
public:
    int NB_SIM;
    int NB_POP;
    int WIDTH=23;
    int HEIGHT=21;
    int cfire = 0;
    int cmine = 0;
    int cturn = 0;
    int last_x = 0;
    int last_y = 0;
    map<int, Sim> enn2;

    vector<Sim> population, lpopulation, next_gen;
    Zombie(){};

    Zombie(int nbp):NB_POP(nbp), NB_SIM(nbp){
        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> dposx(0, WIDTH-1);
        std::uniform_int_distribution<int> dposy(0, HEIGHT-1);
        std::uniform_int_distribution<int> dfire(0, 1);
        std::uniform_int_distribution<int> dstar(0, 1);
        std::uniform_int_distribution<int> dps(0, 2);

        while(population.size() < NB_POP){
            Sim sm;
            sm.x = dposx(rng);
            sm.y = dposy(rng);
            sm.fire = dfire(rng);
            sm.score = -2000000000;
            int t = dps(rng);
            if(t == 0){
                sm.starboard = 0;
                sm.port = 0;
            }
            else if(t == 1){
                sm.starboard = 1;
                sm.port = 0;
            }
            else if(t == 2){
                sm.starboard = 0;
                sm.port = 1;
            }
            
            population.push_back(sm);
        }

    
    }

    void Selection(){
        next_gen = {};
        double sz = (double)population.size()*0.3;
        for(int i = 0;i < sz;++i){
            next_gen.push_back(population[i]);

        }

        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> dng(sz, population.size()-1);

        double sz2 = (double)population.size()*0.2;
        for(int i = 0;i<sz2;++i ){
            next_gen.push_back(population[dng(rng)]);
        }
       

    }

    void NextGen(){

        Selection();

        vector<Sim> children;

        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> dng(0, next_gen.size()-1);
        std::uniform_int_distribution<int> dcross(0, 4);
        std::uniform_int_distribution<int> dfire(0, 1);
        std::uniform_int_distribution<int> dposx(0, WIDTH-1);
        std::uniform_int_distribution<int> dposy(0, HEIGHT-1);
        std::uniform_int_distribution<int> dps(0, 2);

        int nb_mutate = 0;
        int MAX_MUT = next_gen.size();
        while(children.size() < next_gen.size()){
            
            Sim par1 = next_gen[dng(rng)];
            Sim par2 = next_gen[dng(rng)];

            Sim child;
            child.x = par1.x;
            child.y = par2.y;
            child.fire = par1.fire;
            child.starboard = par2.starboard;
            child.port = par1.port;
            child.score = -2000000000;

            if(nb_mutate < MAX_MUT){
                int cross = dcross(rng) ;
                if(cross == 0){
                    child.x = dposx(rng);
               
                 
                }
                else if (cross == 1){
                 
                    child.y = dposy(rng);
                               
                }
                else if (cross == 2){
                    child.fire = dfire(rng);
                }
                else if (cross == 3){
                    int t = dps(rng);
                    if(t == 0){
                        child.starboard = 0;
                        child.port = 0;
                    }
                    else if(t == 1){
                        child.starboard = 1;
                        child.port = 0;
                    }
                    else if(t == 2){
                        child.starboard = 0;
                        child.port = 1;
                    }
                }
                else if (cross == 4){
                    child.x = dposx(rng);
                    child.y = dposy(rng);
                    child.fire = dfire(rng);
                    int t = dps(rng);
                    if(t == 0){
                        child.starboard = 0;
                        child.port = 0;
                    }
                    else if(t == 1){
                        child.starboard = 1;
                        child.port = 0;
                    }
                    else if(t == 2){
                        child.starboard = 0;
                        child.port = 1;
                    }
             
                }

                ++nb_mutate;

            }

            children.push_back(child);

        }

        next_gen.insert(next_gen.end(), children.begin(), children.end());

        population.swap(next_gen);


    }

    double distance(Sim a, Sim b){
        Cube ca = oddr_to_cube(a);
        Cube cb = oddr_to_cube(b);
        return cube_distance(ca, cb);
    }


    string Play(Sim me, vector<Sim> player, vector<Sim> barrel, vector<Sim> ennemies, vector<Sim> canon, vector<Sim> mine,map<int, Sim> enn, int time){

        auto startm = high_resolution_clock::now();;
        
        auto getTime = [&]()-> bool {
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(stop - startm);
            return(duration.count() <= time);
        };

        bool isleader = true;
        for(int i = 0;i < ennemies.size();++i){
            if(ennemies[i].rhum >= me.rhum){
                isleader = false;
                break;
            }

        }

        double dist_friend = distance(me, player[1]); 

        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> dXY(0, 1);
        

        int nb_sim = 0;
        int ind = 0;
        int nb_turn = 0;
        int fire=  0;
        int indf =-1;
        int slower = 0;
        int faster = 0;
        int destroy_mine = 0;
        int indm = -1;
        int fire_ennemy = 0;
        int annule_fire = 0;
        int starb=0, port =0;
        double coll = -2.0;
        int XE, YE;
        int escape = 0;
        while(getTime()){
            fire_ennemy = 0;
            escape = 0;

            Sim sm;
            sm = population[ind];
            sm.firer = 0;
            sm.starbr = 0;
            sm.portr = 0;
            sm.fireen = 0;
            sm.score = 0;
            sm.ismine = 0;

            double malus = 0;
            
            double _minb = 1000000, minb = 0;
            int indb =-1;
            for(int i = 0;i< barrel.size();++i){
                double dist = distance(me, barrel[i]);
                //cerr << dist << endl;
                if(dist < _minb /*&& distme > 3*/){
                    _minb = dist;
                  
                    indb = i;
                }
          
            }
            if(barrel.size() > 0)
                minb = distance(sm, barrel[indb]);


            double minc = 1000000;
            int distmaxcannon = 0;
            bool iscanon = false;
            int indcan = -1;
            for(int i = 0;i< canon.size();++i){
                double dist = distance(me, canon[i]);
                if(dist < minc){
                    minc = dist;
                    indcan = i;
                }
                
            }
            if(canon.size() > 0){
                if (minc < 10){
                    double ds = distance(me, canon[indcan]);
                    double ds2 = distance(sm, canon[indcan]);
                    distmaxcannon = ds;
                    if(ds <= 1 && (me.x == canon[indcan].x && me.y == canon[indcan].y))distmaxcannon = -1000000;
                    if(ds2 <= 1 && (sm.x == canon[indcan].x && sm.y == canon[indcan].y))distmaxcannon += ds2;
                    iscanon = true;
                }
            }
            
            
            
            int score_fire = 0;
            double minf = 1000000;
            indf = -1;
            for(int i = 0;i< ennemies.size();++i){
                double dist = distance(me, ennemies[i]);
                double disten = distance(sm, ennemies[i]);
                if(dist <= 10 ||barrel.size() == 0){
                    if(dist < minf){
                        minf = dist;
                        indf = i;
                    }
                    if(cfire == 0 && sm.fire == 1){
                        sm.fireen = 1;
                        sm.firer = 1;
                        fire_ennemy = 1;
                    }
                    else if(cfire == 1){
                        escape = 1;
                    }
                    
                }
                
                /*if(dist>=10 &&dist <= 15){
                    if(cmine == 0 && barrel.size() == 0){
                        cmine = 1;
                    }
                }*/
            }

            if(fire_ennemy){
                if(barrel.size() > 0)
                    score_fire = 1000000 + distance(sm, ennemies[indf]);
                else
                    score_fire = 1000000 - distance(sm, ennemies[indf]);
                double d = distance(me, ennemies[indf]);
                int dxy = dXY(rng);
                int add = 0;
                double ds = ceil(1.0 + d / 3.0);
                
                /*if(dxy == 1){
                    int xx=enn[ennemies[indf].id].x, yy=enn[ennemies[indf].id].y;
                    if(enn2.count(ennemies[indf].id)>0){

                        Cube ca = oddr_to_cube(ennemies[indf]);
                        Cube cb = oddr_to_cube(enn2[ennemies[indf].id]);
                        Cube cc = cube_subtract(ca , cb);
                        Sim dcs = cube_to_oddr(cc);

                        //xx = enn2[ennemies[indf].id].x;
                        //yy = enn2[ennemies[indf].id].y;

                        //XE = ennemies[indf].x - xx;
                        //YE = ennemies[indf].y - yy;
                        XE = dcs.x;
                        YE = dcs.y;
                        XE = (double)XE * ds;
                        YE = (double)YE * ds;
                    }
                    else{
                        XE = YE = 0;
                    }
                    
                    

                    
                }
                else{
                    XE = 0;
                    YE = 0;
                }*/

                 Sim spf;
                spf = ennemies[indf];

                if (ennemies[indf].y % 2 == 0) {
                    if (ennemies[indf].orientation == 0) {
                        spf.x += ennemies[indf].speed*ds;
                        spf.y += 0;
                    }
                    else if (ennemies[indf].orientation == 1) {
                        spf.x += 0;
                        spf.y += -ennemies[indf].speed*ds;
                    }
                    else if (ennemies[indf].orientation == 2) {
                        spf.x += -ennemies[indf].speed*ds;
                        spf.y += -ennemies[indf].speed*ds;
                    }
                    else if (ennemies[indf].orientation == 3) {
                        spf.x += -ennemies[indf].speed*ds;
                        spf.y += 0;
                    }
                    else if (ennemies[indf].orientation == 4) {
                        spf.x += -ennemies[indf].speed*ds;
                        spf.y += ennemies[indf].speed*ds;
                    }
                    else if (ennemies[indf].orientation == 5) {
                        spf.x += 0;
                        spf.y += ennemies[indf].speed*ds;
                    }

                }
                else {
                    if (ennemies[indf].orientation == 0) {
                        spf.x += ennemies[indf].speed*ds;
                        spf.y += 0;
                    }
                    else if (ennemies[indf].orientation == 1) {
                        spf.x += ennemies[indf].speed*ds;
                        spf.y += -ennemies[indf].speed*ds;
                    }
                    else if (ennemies[indf].orientation == 2) {
                        spf.x += 0;
                        spf.y += -ennemies[indf].speed*ds;
                    }
                    else if (ennemies[indf].orientation == 3) {
                        spf.x += -ennemies[indf].speed*ds;
                        spf.y += 0;
                    }
                    else if (ennemies[indf].orientation == 4) {
                        spf.x += 0;
                        spf.y += ennemies[indf].speed*ds;
                    }
                    else if (ennemies[indf].orientation == 5) {
                        spf.x += ennemies[indf].speed*ds;
                        spf.y += ennemies[indf].speed*ds;
                    }


                }

                sm.XE = spf.x;
                sm.YE = spf.y;
                    

            }

            if(escape){
                Sim sp;
                sp.x = me.x;
                sp.y = me.y;
                if(me.y %2 == 0){
                    if(me.orientation == 0){
                        sp.x += 5;
                        sp.y += 0;
                    }
                    else if(me.orientation == 1){
                        sp.x += 0;
                        sp.y += -5;
                    }
                    else if(me.orientation == 2){
                        sp.x += -5;
                        sp.y += -5;
                    }
                    else if(me.orientation == 3){
                        sp.x += -5;
                        sp.y += 0;
                    }
                    else if(me.orientation == 4){
                        sp.x += -5;
                        sp.y += 5;
                    }
                    else if(me.orientation == 5){
                        sp.x += 0;
                        sp.y += 5;
                    }

                }
                else{
                    if(me.orientation == 0){
                        sp.x += 5;
                        sp.y += 0;
                    }
                    else if(me.orientation == 1){
                        sp.x += 5;
                        sp.y += -5;
                    }
                    else if(me.orientation == 2){
                        sp.x += 0;
                        sp.y += -5;
                    }
                    else if(me.orientation == 3){
                        sp.x += -5;
                        sp.y += 0;
                    }
                    else if(me.orientation == 4){
                        sp.x += 0;
                        sp.y += 5;
                    }
                    else if(me.orientation == 5){
                        sp.x += 5;
                        sp.y += 5;
                    }


                }

                int pos[4][2] = {{0,0}, {22, 0}, {22, 20}, {0, 20}};
                Sim sdt;
                double mindt = 1000000;
                for(int i = 0;i < 4;++i){
                    Sim st;
                    st.x = pos[i][0];
                    st.y = pos[i][1];
                    double dt = distance(st, sm);
                    if(dt < mindt){
                        mindt = dt;
                        sdt.x = pos[i][0];
                        sdt.y = pos[i][1];

                    }
                }



                Sim sd;
                sd.x = (sdt.x + sp.x) / 2;
                sd.y = (sdt.y + sp.y) / 2;

                score_fire = -distance(sm, sd);

            }

            /*if(fire_ennemy && !isleader){
                score_fire = distance(sm, ennemies[indf]);
                if(me.speed == 0){
                    fire = 0;
                    annule_fire = 1;
                    //if(minf > 3&& me.x != 0 && me.x != 22 && me.y != 0 && me.y != 20){
                    //    faster = 1;
                    //}
                }

                                
            }
            else if(fire_ennemy && isleader && minf > 3){
                score_fire = distance(sm, ennemies[indf]);
                fire = 0;
                annule_fire = 1;
                ///*if(minf > 3&& me.x != 0 && me.x != 22 && me.y != 0 && me.y != 20){
                //    faster = 1;
                //}
            }*/

            double minm = 1000000;
            int distmaxmine = 0;
            bool ismine = false;

            for(int i = 0;i< mine.size();++i){
                double dist = distance(me, mine[i]);
                double distsm = distance(sm, mine[i]);
                if(dist <= 5 && dist > 3){
                    
                    if(fire_ennemy == 0 && cfire == 0 && sm.fire == 1){
                        destroy_mine = 1;
                        indm = i;
                        sm.firer = 1;
                        sm.fireen = 0;
                        score_fire = 1000000+distsm;
                    }
                    else{
                        score_fire -= 1000000+distsm;
                        ismine = true;
                        sm.ismine = 1;
                    }
                }

                if(sm.firer == 0 && dist <= 4){
                    score_fire -= 1000000+distsm;
                    ismine = true;
                    sm.ismine = 1;
                    
                }
                
            }

            int score_turn = 0;
            if(!iscanon && !ismine && fire == 0 ){
                double vx = this->last_x - me.x;
                double vy = this->last_y - me.y;

                //cerr << "oncol "<<endl;

                if(vx == 0 && vy == 0){
                    if(me.y %2 == 0){
                        if(me.orientation == 0){
                            vx += 5;
                            vy += 0;
                        }
                        else if(me.orientation == 1){
                            vx += 0;
                            vy += -5;
                        }
                        else if(me.orientation == 2){
                            vx += -5;
                            vy += -5;
                        }
                        else if(me.orientation == 3){
                            vx += -5;
                            vy += 0;
                        }
                        else if(me.orientation == 4){
                            vx += -5;
                            vy += 5;
                        }
                        else if(me.orientation == 5){
                            vx += 0;
                            vy += 5;
                        }
                    }
                    else{
                        if(me.orientation == 0){
                            vx += 5;
                            vy += 0;
                        }
                        else if(me.orientation == 1){
                            vx += 5;
                            vy += -5;
                        }
                        else if(me.orientation == 2){
                            vx += 0;
                            vy += -5;
                        }
                        else if(me.orientation == 3){
                            vx += -5;
                            vy += 0;
                        }
                        else if(me.orientation == 4){
                            vx += 0;
                            vy += 5;
                        }
                        else if(me.orientation == 5){
                            vx += 5;
                            vy += 5;
                        }


                    }

                }
                
                Sim plead_pos;
                if(barrel.size() > 0)
                    plead_pos = barrel[indb];
                else if(indf != -1)
                    plead_pos = ennemies[indf];
                else
                    plead_pos = ennemies[0];

                if(sm.starboard == 1){
                    //cerr << "star " << endl;
                    if(me.y % 2 == 0){

                        if(me.orientation == 0){
                            vx += 0;
                            vy += 5;
                        }
                        else if(me.orientation == 1){
                            vx += 5;
                            vy += 0;
                        }
                        else if(me.orientation == 2){
                            vx += 0;
                            vy += -5;
                        }
                        else if(me.orientation == 3){
                            vx += -5;
                            vy += -5;
                        }
                        else if(me.orientation == 4){
                            vx += -5;
                            vy += 0;
                        }
                        else if(me.orientation == 5){
                            vx += -5;
                            vy += 5;
                        }
                    }
                    else{
                        if(me.orientation == 0){
                            vx += 5;
                            vy += 5;
                        }
                        else if(me.orientation == 1){
                            vx += 5;
                            vy += 0;
                        }
                        else if(me.orientation == 2){
                            vx += 5;
                            vy += -5;
                        }
                        else if(me.orientation == 3){
                            vx += 0;
                            vy += -5;
                        }
                        else if(me.orientation == 4){
                            vx += -5;
                            vy += 0;
                        }
                        else if(me.orientation == 5){
                            vx += 0;
                            vy += 5;
                        }

                    }


                    double col = double((vx * (plead_pos.x - me.x) + vy*(plead_pos.y-me.y))) / 
                    double(sqrt(vx*vx + vy*vy) * 
                    sqrt((plead_pos.x - me.x)* (plead_pos.x - me.x) +  (plead_pos.y - me.y) * (plead_pos.y - me.y))+0.000001);
                    coll = max(coll, col);
                    bool respect = false;

                    if(me.orientation == 0){
                        if(me.y != plead_pos.y){
                            if(plead_pos.y > me.y){
                                respect = true;
                            }
                        }
                    }
                    else if(me.orientation == 3){
                        if(me.y != plead_pos.y){
                            if(plead_pos.y < me.y){
                                respect = true;
                            }
                        }
                    }
                    else if(me.orientation == 2 || me.orientation == 1){
                        if(me.x != plead_pos.x){
                            if(plead_pos.x > me.x){
                                respect = true;
                            }
                        }
                    }
                    else if(me.orientation == 4 || me.orientation == 5){
                        if(me.x != plead_pos.x){
                            if(plead_pos.x < me.x){
                                respect = true;
                            }
                        }
                    }

                    if(col < 0.5 && respect){
                        score_turn = 10000000;
                        sm.starbr = 1;
                    }
                    else{
                        sm.starbr = 0;
                    }


                }
                else if(sm.port == 1){
                    //cerr << "port " << endl;
                    if(me.y % 2 == 0){

                        if(me.orientation == 0){
                            vx += 0;
                            vy += -5;
                        }
                        else if(me.orientation == 1){
                            vx += -5;
                            vy += -5;
                        }
                        else if(me.orientation == 2){
                            vx += -5;
                            vy += 0;
                        }
                        else if(me.orientation == 3){
                            vx += -5;
                            vy += 5;
                        }
                        else if(me.orientation == 4){
                            vx += 0;
                            vy += 5;
                        }
                        else if(me.orientation == 5){
                            vx += 5;
                            vy += 0;
                        }
                    }
                    else{
                        if(me.orientation == 0){
                            vx += 5;
                            vy += -5;
                        }
                        else if(me.orientation == 1){
                            vx += 0;
                            vy += -5;
                        }
                        else if(me.orientation == 2){
                            vx += -5;
                            vy += 0;
                        }
                        else if(me.orientation == 3){
                            vx += 0;
                            vy += 5;
                        }
                        else if(me.orientation == 4){
                            vx += 5;
                            vy += 5;
                        }
                        else if(me.orientation == 5){
                            vx += 5;
                            vy += 0;
                        }

                    }

                    double col = double((vx * (plead_pos.x - me.x) + vy*(plead_pos.y-me.y))) / 
                    double(sqrt(vx*vx + vy*vy) * 
                    sqrt((plead_pos.x - me.x)* (plead_pos.x - me.x) +  (plead_pos.y - me.y) * (plead_pos.y - me.y))+0.000001);
                    coll = max(coll,col);
                    bool respect = false;

                    if(me.orientation == 0){
                        if(me.y != plead_pos.y){
                            if(plead_pos.y < me.y){
                                respect = true;
                            }
                        }
                    }
                    else if(me.orientation == 3){
                        if(me.y != plead_pos.y){
                            if(plead_pos.y > me.y){
                                respect = true;
                            }
                        }
                    }
                    else if(me.orientation == 2 || me.orientation == 1){
                        if(me.x != plead_pos.x){
                            if(plead_pos.x < me.x){
                                respect = true;
                            }
                        }
                    }
                    else if(me.orientation == 4 || me.orientation == 5){
                        if(me.x != plead_pos.x){
                            if(plead_pos.x > me.x){
                                respect = true;
                            }
                        }
                    }

                    if(col < 0.5 && respect){
                        sm.portr = 1;
                        score_turn = 10000000;
                    }
                    else{
                        sm.portr = 0;
                    }

                }

            }

            

            /*if(fire == 0 && cmine == 0 && me.speed < 2 && !ismine && me.x != 0 && me.x != 22 && me.y != 0 && me.y != 20){
                faster = 1;
            }*/

            /*if(fire == 0 && cmine == 0 && me.speed == 2 && ismine){
                slower = 1;
            }*/

            if(sm.ismine){
                sm.score += distmaxcannon;
                sm.firer = 0;
                sm.starbr = 0;
                sm.portr = 0;
                sm.fireen = 0;
                sm.ismine = 1;
              
            }
            else if(sm.firer == 1){
                sm.score += score_fire;
                sm.ismine = 0;
            }
            else if(iscanon){
                sm.score += distmaxcannon;
                sm.firer = 0;
                sm.starbr = 0;
                sm.portr = 0;
                sm.fireen = 0;
                sm.ismine = 0;
            }
            else{
                if(sm.starbr == 1 || sm.portr == 1)
                    sm.score += score_turn;
                if( barrel.size() > 0 && !escape){
                    sm.score += -minb;
                    //if(dist_friend > 5)sm.score += -dist_friend ;
                    if(me.orientation == player[1].orientation && dist_friend <= 5)sm.score+= 1000000;
                }
                else{
                    sm.score += score_fire;
                    //if(dist_friend > 5)sm.score += -dist_friend ;
                    if(me.orientation == player[1].orientation && dist_friend <= 5)sm.score+= 1000000;
                }

                
            }

            population[ind] = sm;

            nb_sim++;
            if(nb_sim == NB_SIM){
                sort(population.begin(), population.end(), [](Sim a, Sim b) -> bool {
                    return a.score > b.score;
                });
                nb_sim = 0;
                this->NextGen();
                this->lpopulation = population;

            }

            nb_turn++;
            ind = (ind + 1) % NB_POP;



        }

        cerr << "col " << coll << endl;
        cerr << "escape " << escape << endl;
        enn2 = enn;
        last_x = me.x;
        last_y = me.y;
        
        
        if(lpopulation[0].firer == 1){
            cfire = 1;
            cmine = 0;
            int xx = 0,yy=0;
           /* xx = ennemies[indf].x+XE;
            yy = ennemies[indf].y+YE;
            if(xx < 0)xx = 0;
            if(xx > 22)xx = 22;
            if(yy < 0)yy = 0;
            if(yy > 20)yy = 20;*/
            if(lpopulation[0].fireen == 1)
                return "FIRE " /*+ to_string(population[0].x) + " " + to_string(population[0].y);//*/+ to_string(lpopulation[0].XE) + " " + to_string(lpopulation[0].YE);
            else
                return "FIRE " + to_string(mine[indm].x) + " " + to_string(mine[indm].y);
        }
        else if(cmine == 1){
            cfire = 0;
            cmine = 1;
            return "MINE";
        }
        else if(lpopulation[0].portr == 1){
            cfire = 0;
            cmine = 0;
            return "PORT";
        }
        else if(lpopulation[0].starbr == 1){
            cfire = 0;
            cmine = 0;
            return "STARBOARD";
        }
        else if(faster == 1){
            cfire = 0;
            cmine = 0;
            return "FASTER";
        }
        else if(slower == 1){
            cfire = 0;
            cmine = 0;
            return "SLOWER";
        }
        else{
            cfire = 0;
            cmine = 0;
            return "MOVE " + to_string(lpopulation[0].x) + " " + to_string(lpopulation[0].y);
            
        }

    }



 };

class Genetics {
public:
    int NB_SIM;
    int NB_POP;
    int DEPTH;
    int WIDTH = 23;
    int HEIGHT = 21;
    int cfire = 0;
    int cmine = 0;
    int cturn = 0;
    int last_x = 0;
    int last_y = 0;
    map<int, Sim> enn2;

    vector<vector<Sim>> population, lpopulation;
    vector<Sim> next_gen;
    Genetics() {};

    Genetics(int nbp, int d) :NB_POP(nbp), NB_SIM(nbp), DEPTH(d) {
        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> dposx(0, WIDTH - 1);
        std::uniform_int_distribution<int> dposy(0, HEIGHT - 1);
        std::uniform_int_distribution<int> dfire(0, 1);
        std::uniform_int_distribution<int> dstar(0, 1);
        std::uniform_int_distribution<int> dps(0, 4);

        for (int i = 0; i < DEPTH; ++i) {
            population.push_back({});
            while (population[i].size() < NB_POP) {


                Sim sm;
                sm.score = -2000000000;
                int t = dps(rng);
                if (t == 0) {
                    sm.faster = 1;
                    sm.slower = 0;
                    sm.fire = 0;
                    sm.starboard = 0;
                    sm.port = 0;
                     
                }
                else if (t == 1) {
                    sm.faster = 0;
                    sm.slower = 1;
                    sm.fire = 0;
                    sm.starboard = 0;
                    sm.port = 0;
                }
                else if (t == 2) {
                    sm.faster = 0;
                    sm.slower = 0;
                    sm.fire = 1;
                    sm.starboard = 0;
                    sm.port = 0;
                }
                else if (t == 3) {
                    sm.faster = 0;
                    sm.slower = 0;
                    sm.fire = 0;
                    sm.starboard = 1;
                    sm.port = 0;
                }
                else if (t == 4) {
                    sm.faster = 0;
                    sm.slower = 0;
                    sm.fire = 0;
                    sm.starboard = 0;
                    sm.port = 1;
                }

                population[i].push_back(sm);
            }
           
        }
            


    }

    void Selection(int depth) {
        next_gen = {};
        double sz = (double)population[depth].size() * 0.3;
        for (int i = 0; i < sz; ++i) {
            next_gen.push_back(population[depth][i]);

        }

        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> dng(sz, population[depth].size() - 1);

        double sz2 = (double)population[depth].size() * 0.2;
        for (int i = 0; i < sz2; ++i) {
            next_gen.push_back(population[depth][dng(rng)]);
        }


    }

    void NextGen(int indgen) {

        Selection(indgen);

        vector<Sim> children;

        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> dng(0, next_gen.size() - 1);
        std::uniform_int_distribution<int> dcross(0, 4);
        std::uniform_int_distribution<int> dfire(0, 1);
        std::uniform_int_distribution<int> dposx(0, WIDTH - 1);
        std::uniform_int_distribution<int> dposy(0, HEIGHT - 1);
        std::uniform_int_distribution<int> dps(0, 2);
        std::uniform_int_distribution<int> dpar(0, 1);

        int nb_mutate = 0;
        int MAX_MUT = next_gen.size();
        while (children.size() < next_gen.size()) {

            Sim par1 = next_gen[dng(rng)];
            Sim par2 = next_gen[dng(rng)];

            Sim child;
            if (dpar(rng) == 0) {
                child.faster = par1.faster;
                child.slower = par1.slower;
                child.fire = par1.fire;
                child.starboard = par1.starboard;
                child.port = par1.port;
                child.speed = par1.speed;
                child.cfire = par1.cfire;
            }
            else {
                child.faster = par2.faster;
                child.slower = par2.slower;
                child.fire = par2.fire;
                child.starboard = par2.starboard;
                child.port = par2.port;
                child.speed = par2.speed;
                child.cfire = par2.cfire;
            }

            child.score = -2000000000;

            if (nb_mutate < MAX_MUT) {
                int t = dcross(rng);

                if (child.speed == 0 && t == 1) {
                    t=0;
                }
                if (child.cfire == 1 && t == 2) {
                    t = 0;
                }
                
                if (t == 0) {
                    child.faster = 1;
                    child.slower = 0;
                    child.fire = 0;
                    child.starboard = 0;
                    child.port = 0;

                }
                else if (t == 1) {
                    child.faster = 0;
                    child.slower = 1;
                    child.fire = 0;
                    child.starboard = 0;
                    child.port = 0;
                }
                else if (t == 2) {
                    child.faster = 0;
                    child.slower = 0;
                    child.fire = 1;
                    child.starboard = 0;
                    child.port = 0;
                }
                else if (t == 3) {
                    child.faster = 0;
                    child.slower = 0;
                    child.fire = 0;
                    child.starboard = 1;
                    child.port = 0;
                }
                else if (t == 4) {
                    child.faster = 0;
                    child.slower = 0;
                    child.fire = 0;
                    child.starboard = 0;
                    child.port = 1;
                }


                ++nb_mutate;

            }
            
            children.push_back(child);

        }

        next_gen.insert(next_gen.end(), children.begin(), children.end());

        population[indgen].swap(next_gen);


    }

    double distance(Sim a, Sim b) {
        Cube ca = oddr_to_cube(a);
        Cube cb = oddr_to_cube(b);
        return cube_distance(ca, cb);
    }


    Sim Simulation(Sim me, string cmd) {
        Sim sp;

        if (cmd == "FASTER") {
            sp = me;
            if (sp.speed < 2)sp.speed++;
            if (me.y % 2 == 0) {
                if (me.orientation == 0) {
                    sp.x += me.speed;
                    sp.y += 0;
                }
                else if (me.orientation == 1) {
                    sp.x += 0;
                    sp.y += -me.speed;
                }
                else if (me.orientation == 2) {
                    sp.x += -me.speed;
                    sp.y += -me.speed;
                }
                else if (me.orientation == 3) {
                    sp.x += -me.speed;
                    sp.y += 0;
                }
                else if (me.orientation == 4) {
                    sp.x += -me.speed;
                    sp.y += me.speed;
                }
                else if (me.orientation == 5) {
                    sp.x += 0;
                    sp.y += me.speed;
                }

            }
            else {
                if (me.orientation == 0) {
                    sp.x += me.speed;
                    sp.y += 0;
                }
                else if (me.orientation == 1) {
                    sp.x += me.speed;
                    sp.y += -me.speed;
                }
                else if (me.orientation == 2) {
                    sp.x += 0;
                    sp.y += -me.speed;
                }
                else if (me.orientation == 3) {
                    sp.x += -me.speed;
                    sp.y += 0;
                }
                else if (me.orientation == 4) {
                    sp.x += 0;
                    sp.y += me.speed;
                }
                else if (me.orientation == 5) {
                    sp.x += me.speed;
                    sp.y += me.speed;
                }


            }

        }
        else if (cmd == "SLOWER") {
            sp = me;
            if (sp.speed > 0)sp.speed--;
            if (me.y % 2 == 0) {
                if (me.orientation == 0) {
                    sp.x += me.speed;
                    sp.y += 0;
                }
                else if (me.orientation == 1) {
                    sp.x += 0;
                    sp.y += -me.speed;
                }
                else if (me.orientation == 2) {
                    sp.x += -me.speed;
                    sp.y += -me.speed;
                }
                else if (me.orientation == 3) {
                    sp.x += -me.speed;
                    sp.y += 0;
                }
                else if (me.orientation == 4) {
                    sp.x += -me.speed;
                    sp.y += me.speed;
                }
                else if (me.orientation == 5) {
                    sp.x += 0;
                    sp.y += me.speed;
                }

            }
            else {
                if (me.orientation == 0) {
                    sp.x += me.speed;
                    sp.y += 0;
                }
                else if (me.orientation == 1) {
                    sp.x += me.speed;
                    sp.y += -me.speed;
                }
                else if (me.orientation == 2) {
                    sp.x += 0;
                    sp.y += -me.speed;
                }
                else if (me.orientation == 3) {
                    sp.x += -me.speed;
                    sp.y += 0;
                }
                else if (me.orientation == 4) {
                    sp.x += 0;
                    sp.y += me.speed;
                }
                else if (me.orientation == 5) {
                    sp.x += me.speed;
                    sp.y += me.speed;
                }


            }

        }
        else if (cmd == "FIRE") {
            sp = me;
            sp.cfire = 1;

            if (me.y % 2 == 0) {
                if (sp.orientation == 0) {
                    sp.x += me.speed;
                    sp.y += 0;
                }
                else if (sp.orientation == 1) {
                    sp.x += 0;
                    sp.y += -me.speed;
                }
                else if (sp.orientation == 2) {
                    sp.x += -me.speed;
                    sp.y += -me.speed;
                }
                else if (sp.orientation == 3) {
                    sp.x += -me.speed;
                    sp.y += 0;
                }
                else if (sp.orientation == 4) {
                    sp.x += -me.speed;
                    sp.y += me.speed;
                }
                else if (sp.orientation == 5) {
                    sp.x += 0;
                    sp.y += me.speed;
                }

            }
            else {
                if (sp.orientation == 0) {
                    sp.x += me.speed;
                    sp.y += 0;
                }
                else if (sp.orientation == 1) {
                    sp.x += me.speed;
                    sp.y += -me.speed;
                }
                else if (sp.orientation == 2) {
                    sp.x += 0;
                    sp.y += -me.speed;
                }
                else if (sp.orientation == 3) {
                    sp.x += -me.speed;
                    sp.y += 0;
                }
                else if (sp.orientation == 4) {
                    sp.x += 0;
                    sp.y += me.speed;
                }
                else if (sp.orientation == 5) {
                    sp.x += me.speed;
                    sp.y += me.speed;
                }


            }

        }
        else if (cmd == "STARBOARD") {
            sp = me;

            if (me.orientation == 0) {
                sp.orientation = 5;
            }
            else if (me.orientation == 1) {
                sp.orientation = 0;
            }
            else if (me.orientation == 2) {
                sp.orientation = 1;
            }
            else if (me.orientation == 3) {
                sp.orientation = 2;
            }
            else if (me.orientation == 4) {
                sp.orientation = 3;
            }
            else if (me.orientation == 5) {
                sp.orientation = 4;
            }

            if (me.y % 2 == 0) {
                if (sp.orientation == 0) {
                    sp.x += me.speed;
                    sp.y += 0;
                }
                else if (sp.orientation == 1) {
                    sp.x += 0;
                    sp.y += -me.speed;
                }
                else if (sp.orientation == 2) {
                    sp.x += -me.speed;
                    sp.y += -me.speed;
                }
                else if (sp.orientation == 3) {
                    sp.x += -me.speed;
                    sp.y += 0;
                }
                else if (sp.orientation == 4) {
                    sp.x += -me.speed;
                    sp.y += me.speed;
                }
                else if (sp.orientation == 5) {
                    sp.x += 0;
                    sp.y += me.speed;
                }

            }
            else {
                if (sp.orientation == 0) {
                    sp.x += me.speed;
                    sp.y += 0;
                }
                else if (sp.orientation == 1) {
                    sp.x += me.speed;
                    sp.y += -me.speed;
                }
                else if (sp.orientation == 2) {
                    sp.x += 0;
                    sp.y += -me.speed;
                }
                else if (sp.orientation == 3) {
                    sp.x += -me.speed;
                    sp.y += 0;
                }
                else if (sp.orientation == 4) {
                    sp.x += 0;
                    sp.y += me.speed;
                }
                else if (sp.orientation == 5) {
                    sp.x += me.speed;
                    sp.y += me.speed;
                }


            }


        }
        else if (cmd == "PORT") {
            sp = me;

            if (me.orientation == 0) {
                sp.orientation = 1;
            }
            else if (me.orientation == 1) {
                sp.orientation = 2;
            }
            else if (me.orientation == 2) {
                sp.orientation = 3;
            }
            else if (me.orientation == 3) {
                sp.orientation = 4;
            }
            else if (me.orientation == 4) {
                sp.orientation = 5;
            }
            else if (me.orientation == 5) {
                sp.orientation = 0;
            }

            if (me.y % 2 == 0) {
                if (sp.orientation == 0) {
                    sp.x += me.speed;
                    sp.y += 0;
                }
                else if (sp.orientation == 1) {
                    sp.x += 0;
                    sp.y += -me.speed;
                }
                else if (sp.orientation == 2) {
                    sp.x += -me.speed;
                    sp.y += -me.speed;
                }
                else if (sp.orientation == 3) {
                    sp.x += -me.speed;
                    sp.y += 0;
                }
                else if (sp.orientation == 4) {
                    sp.x += -me.speed;
                    sp.y += me.speed;
                }
                else if (sp.orientation == 5) {
                    sp.x += 0;
                    sp.y += me.speed;
                }

            }
            else {
                if (sp.orientation == 0) {
                    sp.x += me.speed;
                    sp.y += 0;
                }
                else if (sp.orientation == 1) {
                    sp.x += me.speed;
                    sp.y += -me.speed;
                }
                else if (sp.orientation == 2) {
                    sp.x += 0;
                    sp.y += -me.speed;
                }
                else if (sp.orientation == 3) {
                    sp.x += -me.speed;
                    sp.y += 0;
                }
                else if (sp.orientation == 4) {
                    sp.x += 0;
                    sp.y += me.speed;
                }
                else if (sp.orientation == 5) {
                    sp.x += me.speed;
                    sp.y += me.speed;
                }


            }

        }

        return sp;


    }

    void Decalage_gen(){
        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> dps(0, 4);

        for(int i = 0;i<  DEPTH - 1;++i){
            population[i] = population[i+1];
        }
       
        for(int i = 0;i < NB_POP;++i){
            Sim sm;
            sm.score = -2000000000;
            int t = dps(rng);
            if (t == 0) {
                sm.faster = 1;
                sm.slower = 0;
                sm.fire = 0;
                sm.starboard = 0;
                sm.port = 0;
                    
            }
            else if (t == 1) {
                sm.faster = 0;
                sm.slower = 1;
                sm.fire = 0;
                sm.starboard = 0;
                sm.port = 0;
            }
            else if (t == 2) {
                sm.faster = 0;
                sm.slower = 0;
                sm.fire = 1;
                sm.starboard = 0;
                sm.port = 0;
            }
            else if (t == 3) {
                sm.faster = 0;
                sm.slower = 0;
                sm.fire = 0;
                sm.starboard = 1;
                sm.port = 0;
            }
            else if (t == 4) {
                sm.faster = 0;
                sm.slower = 0;
                sm.fire = 0;
                sm.starboard = 0;
                sm.port = 1;
            }

            population[DEPTH-1][i] = sm;
            
        }
        

    }


    string Play(Sim _me, vector<Sim> player, vector<Sim> barrel, vector<Sim> ennemies, vector<Sim> canon, vector<Sim> mine, map<int, Sim> enn, int turn, int time) {

        auto startm = high_resolution_clock::now();;

        int maxt = -1;
        auto getTime = [&]()-> bool {
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(stop - startm);
            maxt = max(maxt, (int)duration.count());
            return(duration.count() <= time);
        };


        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> dXY(0, 1);

        cerr << "cf " << _me.cfire << endl;

        if(turn > 0&& DEPTH > 1){
            this->Decalage_gen();
        }

        int dist_friend = 1000000;
        int isin = false;
        for(int i = 0;i <player.size();++i){
            if(player[i].id == _me.id)continue;
            double dist = distance(player[i], _me);
            if(dist < dist_friend){
                dist_friend = dist;
                isin = true;
            }
        }
        if(!isin)dist_friend = 0;
     
        Sim lpop;

        Sim me;

        int nb_sim = 0;
        int ind = 0;
        int nb_turn = 0;
        int fire = 0;
        int indf = -1;
        int slower = 0;
        int faster = 0;
        int destroy_mine = 0;
        int indm = -1;
        int fire_ennemy = 0;
        int annule_fire = 0;
        int starb = 0, port = 0;
        double coll = -2.0;
        
        int escape = 0;
        int depth = 0;
        while (getTime()) {
         
            Sim sm;
            sm = population[depth][ind];
            sm.score = 0;
            sm.speed = _me.speed;
         

            Sim sp;

            if (depth == 0) {
                me = _me;
                sm.cfire = _me.cfire;
            }
            else {
                me = population[depth-1][ind];
                sm.cfire = population[depth - 1][ind].cfire;
            }

            /*bool enterfaster = true;
            if((me.x == 0 && (me.orientation == 2 || me.orientation == 3 || me.orientation == 4))||
            (me.x == 22 && (me.orientation == 1 || me.orientation == 0 || me.orientation == 5))||
            (me.y == 0 && (me.orientation == 1 || me.orientation == 2 ))||
            (me.y == 20&& (me.orientation == 4 || me.orientation == 5))||
            (me.x >= 20 || me.x <= 3 || me.y <=3 || me.y >= 17)){
                enterfaster = false;
            }*/

            if (sm.faster == 1 /*&& enterfaster*/) {
                sp = me;
                if (sp.speed < 2)sp.speed++;
                if (me.y % 2 == 0) {
                    if (me.orientation == 0) {
                        sp.x += me.speed;
                        sp.y += 0;
                    }
                    else if (me.orientation == 1) {
                        sp.x += 0;
                        sp.y += -me.speed;
                    }
                    else if (me.orientation == 2) {
                        sp.x += -me.speed;
                        sp.y += -me.speed;
                    }
                    else if (me.orientation == 3) {
                        sp.x += -me.speed;
                        sp.y += 0;
                    }
                    else if (me.orientation == 4) {
                        sp.x += -me.speed;
                        sp.y += me.speed;
                    }
                    else if (me.orientation == 5) {
                        sp.x += 0;
                        sp.y += me.speed;
                    }

                }
                else {
                    if (me.orientation == 0) {
                        sp.x += me.speed;
                        sp.y += 0;
                    }
                    else if (me.orientation == 1) {
                        sp.x += me.speed;
                        sp.y += -me.speed;
                    }
                    else if (me.orientation == 2) {
                        sp.x += 0;
                        sp.y += -me.speed;
                    }
                    else if (me.orientation == 3) {
                        sp.x += -me.speed;
                        sp.y += 0;
                    }
                    else if (me.orientation == 4) {
                        sp.x += 0;
                        sp.y += me.speed;
                    }
                    else if (me.orientation == 5) {
                        sp.x += me.speed;
                        sp.y += me.speed;
                    }


                }

            }
            else if (sm.slower == 1) {
                sp = me;
                if (sp.speed > 0)sp.speed--;
                if (me.y % 2 == 0) {
                    if (me.orientation == 0) {
                        sp.x += me.speed;
                        sp.y += 0;
                    }
                    else if (me.orientation == 1) {
                        sp.x += 0;
                        sp.y += -me.speed;
                    }
                    else if (me.orientation == 2) {
                        sp.x += -me.speed;
                        sp.y += -me.speed;
                    }
                    else if (me.orientation == 3) {
                        sp.x += -me.speed;
                        sp.y += 0;
                    }
                    else if (me.orientation == 4) {
                        sp.x += -me.speed;
                        sp.y += me.speed;
                    }
                    else if (me.orientation == 5) {
                        sp.x += 0;
                        sp.y += me.speed;
                    }

                }
                else {
                    if (me.orientation == 0) {
                        sp.x += me.speed;
                        sp.y += 0;
                    }
                    else if (me.orientation == 1) {
                        sp.x += me.speed;
                        sp.y += -me.speed;
                    }
                    else if (me.orientation == 2) {
                        sp.x += 0;
                        sp.y += -me.speed;
                    }
                    else if (me.orientation == 3) {
                        sp.x += -me.speed;
                        sp.y += 0;
                    }
                    else if (me.orientation == 4) {
                        sp.x += 0;
                        sp.y += me.speed;
                    }
                    else if (me.orientation == 5) {
                        sp.x += me.speed;
                        sp.y += me.speed;
                    }


                }

            }
            else if (sm.cfire == 0 && sm.fire == 1) {
                sp = me;
                
                if (me.y % 2 == 0) {
                    if (sp.orientation == 0) {
                        sp.x += me.speed;
                        sp.y += 0;
                    }
                    else if (sp.orientation == 1) {
                        sp.x += 0;
                        sp.y += -me.speed;
                    }
                    else if (sp.orientation == 2) {
                        sp.x += -me.speed;
                        sp.y += -me.speed;
                    }
                    else if (sp.orientation == 3) {
                        sp.x += -me.speed;
                        sp.y += 0;
                    }
                    else if (sp.orientation == 4) {
                        sp.x += -me.speed;
                        sp.y += me.speed;
                    }
                    else if (sp.orientation == 5) {
                        sp.x += 0;
                        sp.y += me.speed;
                    }

                }
                else {
                    if (sp.orientation == 0) {
                        sp.x += me.speed;
                        sp.y += 0;
                    }
                    else if (sp.orientation == 1) {
                        sp.x += me.speed;
                        sp.y += -me.speed;
                    }
                    else if (sp.orientation == 2) {
                        sp.x += 0;
                        sp.y += -me.speed;
                    }
                    else if (sp.orientation == 3) {
                        sp.x += -me.speed;
                        sp.y += 0;
                    }
                    else if (sp.orientation == 4) {
                        sp.x += 0;
                        sp.y += me.speed;
                    }
                    else if (sp.orientation == 5) {
                        sp.x += me.speed;
                        sp.y += me.speed;
                    }


                }

            }
            else if (sm.starboard == 1) {
                sp = me;

                

                if (me.y % 2 == 0) {
                    if (sp.orientation == 0) {
                        sp.x += me.speed;
                        sp.y += 0;
                    }
                    else if (sp.orientation == 1) {
                        sp.x += 0;
                        sp.y += -me.speed;
                    }
                    else if (sp.orientation == 2) {
                        sp.x += -me.speed;
                        sp.y += -me.speed;
                    }
                    else if (sp.orientation == 3) {
                        sp.x += -me.speed;
                        sp.y += 0;
                    }
                    else if (sp.orientation == 4) {
                        sp.x += -me.speed;
                        sp.y += me.speed;
                    }
                    else if (sp.orientation == 5) {
                        sp.x += 0;
                        sp.y += me.speed;
                    }

                }
                else {
                    if (sp.orientation == 0) {
                        sp.x += me.speed;
                        sp.y += 0;
                    }
                    else if (sp.orientation == 1) {
                        sp.x += me.speed;
                        sp.y += -me.speed;
                    }
                    else if (sp.orientation == 2) {
                        sp.x += 0;
                        sp.y += -me.speed;
                    }
                    else if (sp.orientation == 3) {
                        sp.x += -me.speed;
                        sp.y += 0;
                    }
                    else if (sp.orientation == 4) {
                        sp.x += 0;
                        sp.y += me.speed;
                    }
                    else if (sp.orientation == 5) {
                        sp.x += me.speed;
                        sp.y += me.speed;
                    }


                }

                if (me.orientation == 0) {
                    sp.orientation = 5;
                }
                else if (me.orientation == 1) {
                    sp.orientation = 0;
                }
                else if (me.orientation == 2) {
                    sp.orientation = 1;
                }
                else if (me.orientation == 3) {
                    sp.orientation = 2;
                }
                else if (me.orientation == 4) {
                    sp.orientation = 3;
                }
                else if (me.orientation == 5) {
                    sp.orientation = 4;
                }


            }
            else if (sm.port == 1) {
                sp = me;
               

                if (me.y % 2 == 0) {
                    if (sp.orientation == 0) {
                        sp.x += me.speed;
                        sp.y += 0;
                    }
                    else if (sp.orientation == 1) {
                        sp.x += 0;
                        sp.y += -me.speed;
                    }
                    else if (sp.orientation == 2) {
                        sp.x += -me.speed;
                        sp.y += -me.speed;
                    }
                    else if (sp.orientation == 3) {
                        sp.x += -me.speed;
                        sp.y += 0;
                    }
                    else if (sp.orientation == 4) {
                        sp.x += -me.speed;
                        sp.y += me.speed;
                    }
                    else if (sp.orientation == 5) {
                        sp.x += 0;
                        sp.y += me.speed;
                    }

                }
                else {
                    if (sp.orientation == 0) {
                        sp.x += me.speed;
                        sp.y += 0;
                    }
                    else if (sp.orientation == 1) {
                        sp.x += me.speed;
                        sp.y += -me.speed;
                    }
                    else if (sp.orientation == 2) {
                        sp.x += 0;
                        sp.y += -me.speed;
                    }
                    else if (sp.orientation == 3) {
                        sp.x += -me.speed;
                        sp.y += 0;
                    }
                    else if (sp.orientation == 4) {
                        sp.x += 0;
                        sp.y += me.speed;
                    }
                    else if (sp.orientation == 5) {
                        sp.x += me.speed;
                        sp.y += me.speed;
                    }


                }

                if (me.orientation == 0) {
                    sp.orientation = 1;
                }
                else if (me.orientation == 1) {
                    sp.orientation = 2;
                }
                else if (me.orientation == 2) {
                    sp.orientation = 3;
                }
                else if (me.orientation == 3) {
                    sp.orientation = 4;
                }
                else if (me.orientation == 4) {
                    sp.orientation = 5;
                }
                else if (me.orientation == 5) {
                    sp.orientation = 0;
                }

            }


            double malus = 0;

            double _minb = 1000000, minb = 0;
            int indb = -1;
            for (int i = 0; i < barrel.size(); ++i) {
                double dist = distance(sp, barrel[i]);
                
                if (dist < _minb ) {
                    _minb = dist;

                    indb = i;
                }

            }
            if (barrel.size() > 0) {
                minb = distance(sp, barrel[indb]);
                sm.xb = barrel[indb].x;
                sm.yb = barrel[indb].y;
                sm.distb = distance(me, barrel[indb]);
                
                //cerr << minb <<endl;
            }


            int inzonesh = 0;
            int score_ennemies = 0;
            int no_barrel = 0;
            int shoot = 0;
            double _mine = 1000000, _mine2=1000000;
            int indf = -1, indf2 = -1;
            for (int i = 0; i < ennemies.size(); ++i) {
                double dist = distance(sp, ennemies[i]);
                if (dist < _mine ) {
                    _mine = dist;
                    inzonesh = 1;
                    indf = i;
                }

                /*if(dist < _mine2 && barrel.size() == 0){
                    _mine2 = dist;
                    indf2 = i;
                }*/

            }

            

            if(indf != -1 && _mine <= 10){
                shoot = 1;
                score_ennemies += 300000000+_mine;
                sm.fireen = 1;
                sm.ind_shoot = indf;

                double d = distance(me, ennemies[indf]);
                int dxy = dXY(rng);
                int add = 0;
                double ds = ceil(1.0 + d / 3.0);
                
              
                    /*int xx=enn[ennemies[indf].id].x, yy=enn[ennemies[indf].id].y;
                    if(enn2.count(ennemies[indf].id)>0){

                        Cube ca = oddr_to_cube(ennemies[indf]);
                        Cube cb = oddr_to_cube(enn2[ennemies[indf].id]);
                        Cube cc = cube_subtract(ca , cb);
                        Sim dcs = cube_to_oddr(cc);

                        double xx = dcs.x, yy = dcs.y;
                        if(xx != 0 || yy != 0){
                            double norme = sqrt(xx*xx+yy*yy);
                            xx /= norme;
                            yy /= norme;
                            xx *= (double)ennemies[indf].speed*ds;
                            yy *= (double)ennemies[indf].speed*ds;
                            
                            sm.XE = xx;
                            sm.YE = yy;
                        }
                        else{
                            sm.XE = sm.YE = 0;
                        }
                       
                    }
                    else{
                        sm.XE = sm.YE = 0;
                    }*/

                Sim spf;
                spf = ennemies[indf];

                if (ennemies[indf].y % 2 == 0) {
                    if (ennemies[indf].orientation == 0) {
                        spf.x += ennemies[indf].speed*ds;
                        spf.y += 0;
                    }
                    else if (ennemies[indf].orientation == 1) {
                        spf.x += 0;
                        spf.y += -ennemies[indf].speed*ds;
                    }
                    else if (ennemies[indf].orientation == 2) {
                        spf.x += -ennemies[indf].speed*ds;
                        spf.y += -ennemies[indf].speed*ds;
                    }
                    else if (ennemies[indf].orientation == 3) {
                        spf.x += -ennemies[indf].speed*ds;
                        spf.y += 0;
                    }
                    else if (ennemies[indf].orientation == 4) {
                        spf.x += -ennemies[indf].speed*ds;
                        spf.y += ennemies[indf].speed*ds;
                    }
                    else if (ennemies[indf].orientation == 5) {
                        spf.x += 0;
                        spf.y += ennemies[indf].speed*ds;
                    }

                }
                else {
                    if (ennemies[indf].orientation == 0) {
                        spf.x += ennemies[indf].speed*ds;
                        spf.y += 0;
                    }
                    else if (ennemies[indf].orientation == 1) {
                        spf.x += ennemies[indf].speed*ds;
                        spf.y += -ennemies[indf].speed*ds;
                    }
                    else if (ennemies[indf].orientation == 2) {
                        spf.x += 0;
                        spf.y += -ennemies[indf].speed*ds;
                    }
                    else if (ennemies[indf].orientation == 3) {
                        spf.x += -ennemies[indf].speed*ds;
                        spf.y += 0;
                    }
                    else if (ennemies[indf].orientation == 4) {
                        spf.x += 0;
                        spf.y += ennemies[indf].speed*ds;
                    }
                    else if (ennemies[indf].orientation == 5) {
                        spf.x += ennemies[indf].speed*ds;
                        spf.y += ennemies[indf].speed*ds;
                    }


                }

                sm.XE = spf.x;
                sm.YE = spf.y;
                    

               

            }
            else{
                /*if(_mine <= 2)
                    score_ennemies = 300000000+_mine;
                else*/
                    score_ennemies = 300000000-_mine;
            }

            
            double _minc = 1000000, minc = 0;
            int indc = -1;
            int score_canon = 0;
            int canonhere = 0;
            for (int i = 0; i < canon.size(); ++i) {
                double dist = distance(sp, canon[i]);
                
                if (dist < _minc ) {
                    _minc = dist;

                    indc = i;
                }

            }
            if(indc != -1){
                canonhere = 1;

                Sim da=sp, db=sp ;
                if(sp.y % 2 == 0){
                    if(sp.orientation == 0){
                        da.x++;
                        db.x--;
                    }
                    else if(sp.orientation == 1){
                        da.y--;
                        db.x--;
                        db.y++;
                    }
                    else if(sp.orientation == 2){
                        da.x--;
                        da.y--;
                        db.y++;
                    }
                    else if(sp.orientation == 3){
                        da.x--;
                        db.x++;
                    }
                    else if(sp.orientation == 4){
                        da.x--;
                        da.y++;
                        db.y--;
                    }
                    else if(sp.orientation == 5){
                        da.y++;
                        db.x--;
                        db.y--;
                    }
                }
                else{
                    if(sp.orientation == 0){
                        da.x++;
                        db.x--;
                    }
                    else if(sp.orientation == 1){
                        da.x++;
                        da.y--;
                        db.y++;
                    }
                    else if(sp.orientation == 2){
                        da.y--;
                        db.x++;
                        db.y++;
                    }
                    else if(sp.orientation == 3){
                        da.x--;
                        db.x++;
                    }
                    else if(sp.orientation == 4){
                        da.y++;
                        db.x++;
                        db.y--;
                    }
                    else if(sp.orientation == 5){
                        da.x++;
                        da.y++;
                        db.y--;
                    }
                }


                score_canon = 30000000 + _minc + distance(da, canon[indc]) + distance(db, canon[indc]);
            }

            double _minm = 1000000, minm = 0;
            int indm = -1;
            int score_mine = 0;
            int score_shoot_mine = 0;
            int zone_mine = 0;
            int shoot_mine = 0;
        
            for (int i = 0; i < mine.size(); ++i) {
                double dist = distance(me, mine[i]);
                
                if (dist < _minb ) {
                    _minm = dist;

                    indm = i;
                }



            }

            int annule_barrel = 0;
            if(indm != -1){
                if(indb != -1){
                    double dbm = distance(barrel[indb], mine[indm]);
                    if(dbm <= 3){
                        annule_barrel = 1;
                    }
                }
                if(shoot == 0 && sm.cfire == 0 && sm.fire == 1 && _minm >= 4 ){
                    score_shoot_mine = 200000000-_minm;
                    sm.ind_mine = indm;
                    sm.shoot_mine = 1;
                    shoot_mine = 1;
                }
                else{
                    Sim da=sp, db=sp ;
                    
                    if(sp.y % 2 == 0){
                        if(sp.orientation == 0){
                            da.x++;
                            db.x--;
                        }
                        else if(sp.orientation == 1){
                            da.y--;
                            db.x--;
                            db.y++;
                        }
                        else if(sp.orientation == 2){
                            da.x--;
                            da.y--;
                            db.y++;
                        }
                        else if(sp.orientation == 3){
                            da.x--;
                            db.x++;
                        }
                        else if(sp.orientation == 4){
                            da.x--;
                            da.y++;
                            db.y--;
                        }
                        else if(sp.orientation == 5){
                            da.y++;
                            db.x--;
                            db.y--;
                        }
                    }
                    else{
                        if(sp.orientation == 0){
                            da.x++;
                            db.x--;
                        }
                        else if(sp.orientation == 1){
                            da.x++;
                            da.y--;
                            db.y++;
                        }
                        else if(sp.orientation == 2){
                            da.y--;
                            db.x++;
                            db.y++;
                        }
                        else if(sp.orientation == 3){
                            da.x--;
                            db.x++;
                        }
                        else if(sp.orientation == 4){
                            da.y++;
                            db.x++;
                            db.y--;
                        }
                        else if(sp.orientation == 5){
                            da.x++;
                            da.y++;
                            db.y--;
                        }
                    }

                    zone_mine= 1;
                    score_mine = 100000000+_minm + distance(da, mine[indm]) + distance(db, mine[indm]);
                }
               

            }
                                    
            sm.orientation = sp.orientation;
            sm.speed = sp.speed;
            
            if(sm.slower == 1 && sm.speed == 0)
                sm.score = -2000000000;
            else if (sm.cfire == 1 && sm.fire == 1)
                sm.score = -2000000000;
            else if (sm.cfire == 0 && sm.fire == 1 && inzonesh==1 && zone_mine == 0 &&_mine <= 10){
                sm.score = score_ennemies;
                //cerr << "in" << endl;
            }
            else if (sm.cfire == 0 && sm.fire == 1 && shoot_mine == 1){
                sm.score = score_shoot_mine;
                //cerr << "in" << endl;
            }
            else if (zone_mine == 1){
                sm.score = score_mine;
            }
            else if(canonhere && sm.cfire == 0){
                sm.score = score_canon;
            }
            else{
                if(barrel.size() > 0 && zone_mine == 0 && sm.distb <= 2){
                    sm.score = 1000-minb;
                    sm.moveb = 1;
                    /*
                    if(sm.distb <= 2 && _mine >10 && annule_barrel == 0)sm.moveb = 1;
                    else sm.moveb = 0;*/
                }
                else{
                    sm.score = score_ennemies;
                    Sim sd ;
                    sd.x = 11;
                    sd.y = 10;
                    //sm.score += 1000000-distance(sd, sp);
                    if(dist_friend >= 3)sm.score += 10000 + dist_friend;
                    //else sm.score += 10000 + dist_friend;
                }

                
            }
      
        

        
            if (sm.starboard == 1 || sm.port == 1) {
                sm.x = me.x;
                sm.y = me.y;
                
                
                

            }
            else {
                sm.x = sp.x;
                sm.y = sp.y;
                if (sm.cfire == 0 && sm.fire == 1) {
                    sm.cfire = 1;
                }
                
            }

            if(sm.x >= 20 || sm.x <= 3 || sm.y <=3 || sm.y >= 17){
                sm.move = 1;
                if(sm.x <= 3){
                    sm.x = 4;
                }
                if(sm.x >= 20){
                    sm.x = 19;
                }
                if(sm.y <= 3){
                    sm.y = 4;
                }
                if(sm.y >= 17){
                    sm.y = 16;
                }
                
            }
            else{
                sm.move = 0;
            }
              

            population[depth][ind] = sm;
          
            nb_sim++;
            if (nb_sim == NB_SIM) {
                nb_sim = 0;

                
                sort(population[depth].begin(), population[depth].end(), [](Sim a, Sim b) -> bool {
                    return a.score > b.score;
                    });

                this->NextGen(depth);
                                

                depth++;
                if (depth == DEPTH) {
                    lpop = population[0][0];
                    //lpopulation = population;
                    depth = 0;
                }
            }

                           

            ind = (ind + 1) % NB_POP;
            nb_turn++;
            



        }

        //population = lpopulation;
        //lpop = population[0][0];

        enn2 = enn;
        cerr << nb_turn << endl;
        cerr << "time " << maxt <<  " " << lpop.score << endl;
                
        if(lpop.moveb == 1){
            return "MOVE " + to_string(lpop.xb) + " " + to_string(lpop.yb);
        }
        else if(lpop.move == 1){
            return "MOVE " + to_string(lpop.x) + " " + to_string(lpop.y);
        }
        else if (lpop.faster == 1) {
            return "FASTER";
        }
        else if (lpop.slower == 1) {
            return "SLOWER";
        }
        else if(lpop.fireen == 1){
                /*if(ennemies[lpop.ind_shoot].x+lpop.XE < 0 || ennemies[lpop.ind_shoot].y+lpop.YE){
                    lpop.XE = 0;
                    lpop.YE = 0;
                }*/
                return  "FIRE " + to_string(lpop.XE) +" " + to_string(lpop.YE);
            
        }
        else if(lpop.shoot_mine == 1){
            return  "FIRE " + to_string(mine[lpop.ind_mine].x) +" " + to_string(mine[lpop.ind_mine].y);
        }
        else if (lpop.starboard == 1) {
            return "STARBOARD";
        }
        else if (lpop.port == 1) {
            return "PORT";
        }
        else{
            return "WAIT";
        }

        

    }



};






int main()
{
    
    vector<bool> prec_fire(3, false);
    vector<bool> fire(3, false);
    map<int, Zombie> carib;
    map<int, Genetics> gen;
    map<int, int> cfire;
    
    // game loop
    int turn = 0;
    while (1) {
        int my_ship_count; // the number of remaining ships
        cin >> my_ship_count; cin.ignore();
        int entity_count; // the number of entities (e.g. ships, mines or cannonballs)
        cin >> entity_count; cin.ignore();
        
        vector<Sim>my_ent;
        vector<int> speed;
        vector<Sim>ennemies;
        vector<Sim> barrel;
        vector<Sim> mine, canonball;
        int nb_boat = 0;
        map<int, Sim> enn;
        for (int i = 0; i < entity_count; i++) {
            int entity_id;
            string entity_type;
            int x;
            int y;
            int arg_1;
            int arg_2;
            int arg_3;
            int arg_4;
            cin >> entity_id >> entity_type >> x >> y >> arg_1 >> arg_2 >> arg_3 >> arg_4; cin.ignore();
            
            if(turn == 0){
                carib[entity_id] = Zombie(100);
                gen[entity_id] = Genetics(100, 4);
                cfire[entity_id] = 0;
            }
        
            if(entity_type == "BARREL"){
                Sim sm;
                sm.x = x;
                sm.y = y;
                sm.rhum = arg_1;
                barrel.push_back(sm);
            }
            else if(entity_type == "SHIP" && arg_4 == 1){
                Sim sm;
                sm.id = entity_id;
                sm.x = x;
                sm.y = y;
                sm.orientation = arg_1;
                sm.speed = arg_2;
                sm.rhum = arg_3;
                my_ent.push_back(sm);
                nb_boat++;
                
            }
            else if(entity_type == "SHIP" && arg_4 == 0){
                Sim sm;
                sm.id = entity_id;
                sm.x = x;
                sm.y = y;
                sm.orientation = arg_1;
                sm.speed = arg_2;
                sm.rhum = arg_3;
                ennemies.push_back(sm);
                enn[entity_id] = sm;
            
            }
            else if(entity_type == "MINE"){
                Sim sm;
                sm.x = x;
                sm.y = y;
                mine.push_back(sm);

            }
            else if(entity_type == "CANNONBALL"){
                Sim sm;
                sm.x = x;
                sm.y = y;
                sm.id_ship_canon = arg_1;
                sm.nb_turn_bimpact = arg_2;
                canonball.push_back(sm);

            }

            
        
        }

        string res;
        for (int i = 0; i < nb_boat; i++) {
            cerr << my_ent[i].id << endl;
            my_ent[i].cfire = cfire[my_ent[i].id];
            //carib[my_ent[i].id].cfire = cfire[my_ent[i].id];
            if(my_ent[i].x >= 20 || my_ent[i].x <= 3 || my_ent[i].y <=3 || my_ent[i].y >= 17){
                res = carib[my_ent[i].id].Play(my_ent[i], my_ent, barrel, ennemies, canonball, mine, enn, 40/nb_boat);
                cout << res << endl;
            }
            else{
                res = gen[my_ent[i].id].Play(my_ent[i], my_ent, barrel, ennemies,canonball, mine, enn, turn, 40/nb_boat);
                cout << res << endl;
            }
            if (res.substr(0, 4) == "FIRE") {
                cfire[my_ent[i].id] = 1;
            }
            else {
                cfire[my_ent[i].id] = 0;
            }
            

        }


        turn++;
        
    }
}