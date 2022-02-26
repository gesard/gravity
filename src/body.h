#include <math.h>
#include <iostream>

using namespace::std;

namespace astro

{

typedef double ld;

const ld c = 3e8;
const ld G = 6.67e-11;
ld DeltaTime = 1e3;

template <typename number> number sqr(number a){
    return a*a;
}

struct vec2{
    ld x = 0, y = 0;
    vec2(){};
    vec2(ld x, ld y){
        this->x = x;
        this->y = y;
    };

    vec2 operator+ (vec2 v2){
        return {x+v2.x, y+v2.y};
    }

    void operator+= (vec2 v2){
        *this = *this + v2;
    }

    vec2 operator- (vec2 v2){
        return {x-v2.x, y-v2.y};
    }

    vec2 operator* (ld number){
        return {x*number, y*number};
    }

    void FromPolar(ld theta, ld radius){
        x = radius * cosl(theta);
        y = radius * sinl(theta);
    }

    vec2 FromPolarVec2(){
        return vec2(y * cosl(x), y * sinl(x));
    }

    void ToPolar(ld x, ld y){
        ld temp_y = sqrtl(x*x+y*y);
        if (x > 0)
            x = atan(y/x)+M_PI/2;
        else
            x = atan(y/x)+M_PI*3/2;
        y = temp_y;
    }
    ld Distance2(vec2 vc2){
        return sqr(x - vc2.x) + sqr(y - vc2.y);
    }

    void Print(){
        cout << "{ " << x << " | " << y << " }";
    }
};


struct body{
    typedef float ld;
    vec2 coord;
    vec2 speed = {};
    vec2 acceleration = {};
    ld radius;
    ld energy;
    body(){};
    body(ld x, ld y){
        coord.x = x;
        coord.y = y;
    };
    body(ld x, ld y, ld mass){
        coord.x = x;
        coord.y = y;
        energy = mass*c*c;
    };
    body(ld x, ld y, ld mass, vec2 PolarSpeed){
        coord.x = x;
        coord.y = y;
        energy = mass*c*c;
        speed = PolarSpeed.FromPolarVec2();
    }
    body(ld x, ld y, ld mass, ld xv, ld yv){
        coord.x = x;
        coord.y = y;
        energy = mass * c*c;
        speed.x= xv;
        speed.y = yv;
    }
    ld Mass(){
        return energy/sqr(c);
    }
    void UpdCoord(){
        coord += speed*DeltaTime;
    }
    void UpdSpeed(){
        speed += acceleration*DeltaTime;
    }

    void NewPower(body SBody){
        ld Power = G * (Mass()/coord.Distance2(SBody.coord)*SBody.Mass());
        if (abs(Power) == INFINITY)
            Power = 0;
        ld NewAcceleration = Power / Mass();
        ld theta = atan2l((SBody.coord-coord).y, (SBody.coord-coord).x);
        acceleration += vec2(theta, NewAcceleration).FromPolarVec2();   
    }

    void upd(){
        UpdCoord();
        UpdSpeed();
        acceleration = {};
    }

    void Print(){
        cout << "coord: ";
        coord.Print();
        cout << "\n";
        cout << "speed: ";
        speed.Print();
        cout << "\n";
        cout << "acceleration: "; acceleration.Print();
        cout << "\n";
        cout << "mass:" << Mass() << "\n";
    }
};

}
