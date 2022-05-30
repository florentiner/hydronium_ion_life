//
// Created by Данил Ежов on 27.05.2022.
//

#ifndef WATER_TRAJ_ATOM_H
#define WATER_TRAJ_ATOM_H

#include <string>
#include <cmath>



class Atom {
private:
    float x = 0;
    float y = 0;
    float z = 0;
    int name = -1;
    int type = -1;
    Atom* O_name = NULL;
    int H_count = 0;
    std::vector<Atom> H_arr;
public:
    Atom() {
        this->x = 0;
        this->y = 0;
        this->z = 0;
        this->name = -1;
        this->type = -1;
        this->O_name = NULL;
        this->H_count = 0;
    }

    Atom( std::vector<std::string> re)
    {
        this->name = stoi(re[0]);
        this->type = stoi(re[1]);
        this->x =stof(re[2]);
        this->y = stof(re[3]);
        this->z = stof(re[4]);
        if (this->type == 1){
            O_name = NULL;
        }
        else{
            H_count = 0;
        }
    }

    float get_x(){
        return x;
    }
    float get_y(){
        return y;
    }
    float get_z(){
        return z;
    }
    float get_name(){
        return name;
    }

    int get_H_count(){
        return H_count;
    }

    float* coord(){
        float ret_cord[3];
        ret_cord[0] = x;
        ret_cord[1] = y;
        ret_cord[2] = z;
        return ret_cord;
    }

    bool up_H_count(Atom H){
        this->H_arr.push_back(H);
        this->H_count+=1;
        if (this->H_count >= 3){
            return true;
        }
        else{
            return false;
        }
    }

    bool chec_O_name(Atom* O){
        if (this->O_name == NULL){
            this->O_name = O;
            return false;
        }
        else if (this->O_name->get_name() != (*O).get_name()){
            this->O_name = O;
            return true;
        }
        else{
            return false;
        }
    }

    std::vector<Atom> get_H(){
        return this->H_arr;
    }

    int get_O_name(){
        return this->O_name->get_name();
    }

    void del_atom(int H_num){
        this->H_count--;
        for (int i = 0; i < H_arr.size(); ++i) {
            if(H_arr[i].get_name() ==H_num){
                H_arr.erase(H_arr.begin() + i);
            }
        }
    }

    float dist_bt_atoms( Atom O, float base[3]){
        float lenght = 0;
        for (int i = 0; i < 3; ++i) {
            float r = base[i];
            float distance = abs(O.coord()[i] * r - this->coord()[i] * r);
            if(distance < r/2){
                lenght += pow(distance, 2);
            }
            else{
                lenght += pow(r - distance, 2);
            }
        }
        return sqrt(lenght);
    }

    Atom* get_O(){
        return this->O_name;
    }

    void set_cord(std::string x,std::string y,std::string z){
        this->x = stof(x);
        this->y = stof(y);
        this->z = stof(z);
    }
};



#endif //WATER_TRAJ_ATOM_H
