%module trader
%{
    #define SWIG_FILE_WITH_INIT
    #include "trader.h"
%}

%include std_string.i
%include std_vector.i
%include std_map.i

%template() std::vector<std::string>;
%template() std::map<std::string, int>;
%template() std::map<std::string, double>;

%include "trader.h"
