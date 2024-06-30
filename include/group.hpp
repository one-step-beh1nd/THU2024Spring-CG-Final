#ifndef GROUP_H
#define GROUP_H
#include "object3d.hpp"
#include "ray.hpp"
#include "hit.hpp"
#include <iostream>
#include <vector>

using namespace std;

// 本文件自行实现
class Group : public Object3D {

public:

    Group() {

    }

    explicit Group (int num_objects) {
       list = vector<Object3D*>(num_objects);
       E_object = vector<Object3D*>(num_objects);
       E_num = 0;
    }

    ~Group() override {

    }

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        bool crossed = false;
        for(int i=0;i<list.size();i++)
        {
            if(list[i]->intersect(r,h,tmin))
            {
                crossed = true;
            }
        }
        return crossed;
    }

    void drawGL(){
        for(int i=0;i<list.size();i++)
        {
            list[i]->drawGL();
        }
    }

    void addObject(int index, Object3D *obj) {
        list[index] = obj;
    }

    int getGroupSize() {
        return list.size();
    }

    void addE_object(int index, Object3D *obj){
        E_object[index] = obj;
        E_num++;
    }

    vector<Object3D*> getE_object(){
        return E_object;
    }
    int getE_num(){
        return E_num;
    }
private:
    vector<Object3D*> list;
    vector<Object3D*> E_object;
    int E_num;
};

#endif
	
