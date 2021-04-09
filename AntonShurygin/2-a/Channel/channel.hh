#ifndef __CHANNEL_H__
#define __CHANNEL_H__

#include "../Matrix/matrix.hh"

namespace Linear_space
{

class Channel final
{
private: 
    std::vector<Matrix> channels;

    uint height_, widht_;
    uint n_chnls_;

public:

    Channel(uint height, uint widht, uint n_chnls);
    Channel(const std::vector<Matrix>& matr_vec, uint height, uint widht);
    Channel(uint height, uint widht, uint n_chnls, float data);
    Channel(const Channel& chnl);

    uint get_height() const;

    uint get_widht()  const;
  
    uint get_n_chnls() const;

    const Matrix& get_matr(size_t index);

    Matrix& operator[] (size_t index);

    Channel& operator= (const Channel& chnl);

    bool operator== (const Channel& chnl);
    
};

std::ostream& operator << (std::ostream& os, Channel& chnl);

}

#endif /* __CHANNEL_H__ */