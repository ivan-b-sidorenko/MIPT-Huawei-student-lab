#include "../Matrix/matrix.hh"


namespace Linear_space
{

class Channel final
{
private: 
    std::vector<Matr_flt> channels;

    uint height_, widht_;
    uint n_chnls_;

public:

    Channel(uint height, uint widht, uint n_chnls);
    Channel(const std::vector<Matr_flt>& matr_vec, uint height, uint widht);
    Channel(const Channel& chnl) = delete;

    uint get_height() const;

    uint get_widht()  const;
  
    uint get_n_chnls() const;

    const Matr_flt& get_matr(size_t index);

    const Matr_flt& operator[] (size_t index);

    Channel& operator= (const Channel& chnl);

    bool operator== (const Channel& chnl);

    

};

std::ostream& operator << (std::ostream& os, Channel& chnl);

}