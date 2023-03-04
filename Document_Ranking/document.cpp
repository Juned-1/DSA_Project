#include"document.hpp"
document::~document(){
    delete [] name;
    delete [] text;
}
