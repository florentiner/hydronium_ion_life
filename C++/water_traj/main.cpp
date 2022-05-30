#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "Atom.h"

int is_ok_H(std::vector<Atom> ar_o){
    int O_count = 0;
    int ar_i_O = 0;
    for(Atom O: ar_o){
        ar_i_O++;
        if (O.get_H_count() >= 3){
            std::cout << ar_i_O << std::endl;
            O_count++;
        }
    }
    return O_count;
}

std::vector<std::string> split(std::string str, char delimiter) {
    std::vector<std::string> internal;
    std::stringstream ss(str); // Turn the string into a stream.
    std::string tok;

    while(getline(ss, tok, delimiter)) {
        internal.push_back(tok);
    }

    return internal;
}

int in_vector(std::string name,std::vector<Atom> H_arr){
    int i = 0;
    for (Atom H: H_arr){
        if (H.get_name() == stoi(name)){
            return i + 1;
        }
        i++;
    }
    return 0;
}

int main() {
    std::vector<int> life_ar;
    std::vector<int> frame_time_ar;
    float water_lentg = 0.96;
    int frame_time = 0;
    std::fstream newfile;
    std::string start_frame = "ITEM: BOX BOUNDS pp pp pp";
    std::string end_frame ="ITEM: TIMESTEP";
    std::string start_coord = "ITEM: ATOMS id type xs ys zs";
    newfile.open("/Users/danilezov/CLionProjects/water_traj/traj_b.dmp",std::ios::in); //open a file to perform read operation using file object
    if (newfile.is_open()){ //checking whether the file is open
        std::string tp;
        bool open_frame = false;
        bool first_frame = true;
        bool start_frame_coord = false;
        std::vector<Atom> h_arr;
        std::vector<Atom> o_arr;
        int open_frame_count = 0;
        std::vector<Atom> jump_H;
        std::vector<Atom> h_chech_arr;
        float frame_lin[3];
        int life_time = 0;
        while(getline(newfile, tp)){
            if (start_frame == tp){
                open_frame = true;
            }
            else if(start_coord == tp){
                start_frame_coord = true;
            }
            else if ((open_frame) and (!start_frame_coord)){
                std::vector<std::string> re = split(tp, ' ');
                frame_lin[open_frame_count] = std::stof( re[1]);
                open_frame_count++;
            }
            else if (start_frame_coord) {
                std::vector<std::string> re = split(tp, ' ');
                if (first_frame) {
                        if (re[1] == "1") {
                            Atom H = Atom(re);
                            h_arr.push_back(H);
                        }
                        if (re[1] == "2") {
                            Atom O = Atom(re);
                            o_arr.push_back(O);
                        }
                    }
                else{
                    if ((re[1] == "1") and (in_vector(re[0], jump_H) )) {
                        Atom H = jump_H[in_vector(re[0], jump_H) - 1];
                        H.set_cord(re[2], re[3], re[4]);
                        h_chech_arr.push_back(H);
                    }
                }
            }
            if (end_frame == tp){
                if (first_frame) {
                    for (int i = 0; i < h_arr.size(); ++i) {
                        float min_dist = 10000;
                        Atom* H = &h_arr[i];
                        Atom* best_O;
                        int max_i;
                        for (int j = 0; j < o_arr.size(); ++j) {
                            Atom* O = &o_arr[j];
                            float dist = (*H).dist_bt_atoms(*O, frame_lin);
                            if (dist < min_dist){
                                min_dist = dist;
                                max_i = j;
                                best_O = O;
                            }
                        }
                        (*H).chec_O_name(best_O);
                        if((*best_O).up_H_count(*H)){
                            jump_H = (*best_O).get_H();
                        }

                    }

                    first_frame = false;
                }
                else{
                    for (int i = 0; i < h_chech_arr.size(); ++i) {
                        float min_dist = 10000;
                        Atom* H = &h_chech_arr[i];
                        Atom* best_O;
                        for (int j = 0; j < o_arr.size(); ++j) {
                            Atom* O = &o_arr[j];
                            float dist = (*H).dist_bt_atoms(*O, frame_lin);
                            if (dist < min_dist){
                                min_dist = dist;
                                best_O = O;
                            }
                        }
                        if (((*H).get_O() != best_O) and ((*H).dist_bt_atoms(*best_O, frame_lin) <= water_lentg)){
//                            std::cout << (*H).dist_bt_atoms(*best_O, frame_lin) <<std::endl;
//                            std::cout << (*H).dist_bt_atoms(*(*H).get_O(), frame_lin) <<std::endl;
                            Atom* prev = (*H).get_O();
                            std::cout << (*prev).get_name() <<std::endl;
                            std::cout << (*best_O).get_name() <<std::endl;
                            (*prev).del_atom((*H).get_name());
                            (*H).chec_O_name(best_O);
                            (*best_O).up_H_count(*H);
                            jump_H = (*best_O).get_H();
                            life_ar.push_back(life_time);
                            life_time = 0;
                            break;
                        }

                    }


                }
                frame_time++;
                life_time++;
                std::fill( std::begin( frame_lin ), std::end( frame_lin ), 0 );
                //int H_numb = is_ok_H(o_arr);
                open_frame_count = 0;
                open_frame = false;
                start_frame_coord = false;
                first_frame = false;
                h_chech_arr.clear();
            }
        }
        newfile.close(); //close the file object.
    }
    std::ofstream res_file;
    res_file.open ("res_mod.txt");
    for(int el:life_ar){
        res_file << el << ", ";
    }
    std::cout << frame_time << std::endl;
    res_file.close();
    return 0;
}
