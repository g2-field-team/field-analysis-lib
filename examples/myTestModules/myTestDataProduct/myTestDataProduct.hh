#ifndef MYTESTDATAPRODUCT_HH
#define MYTESTDATAPRODUCT_HH


#include <vector>

namespace myTestModules {

    struct myTestDataProduct {
      std::vector <int> TimeStamp;
      std::vector <int> Position;
      std::vector <int> Velocity;

        myTestDataProduct()
        {}

        virtual ~myTestDataProduct(){};

        // ROOT doesn't need to know the rest
/*#ifndef __GCCXML__
        myTestDataProduct()
        {}
#endif // __GCCXML__
*/
    }; //end of myTestDataProduct struct

    typedef std::vector<myTestDataProduct> myTestDataProductCollection;
} 

#endif //  MYTESTDATAPRODUCT_HH
