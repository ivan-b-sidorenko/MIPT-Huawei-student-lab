#ifndef __TENSOR_H__
#define __TENSOR_H__

#include "../Channel/channel.hh"

namespace Linear_space
{

class Tensor_4d final
{
private: 
    std::vector<Channel> butchs;

    uint height_, widht_;
    uint n_chnls_;
    uint n_butch_;

public:

    Tensor_4d() {};
    Tensor_4d(uint height, uint widht, uint n_chnls, uint n_butch);
    Tensor_4d(const std::vector<Channel>& chnl_vec, uint height, uint widht, uint n_chnls);
    Tensor_4d(uint height, uint widht, uint n_chnls, uint n_butch, float data);
    Tensor_4d(uint height, uint widht, uint n_chnls, uint n_butch, enum Mode mode);

    Tensor_4d(const Tensor_4d& tensor);
    ~Tensor_4d() {};

    uint get_height() const;

    uint get_widht()  const;
  
    uint get_n_chnls() const;

    uint get_n_butch() const;

    const Channel& get_chnl(size_t index);

    Channel& operator[] (size_t index);

    Tensor_4d& operator= (const Tensor_4d& chnl);
    
    bool operator== (const Tensor_4d& chnl);

};

std::ostream& operator << (std::ostream& os, Tensor_4d& tensor);

}

#endif /* __TENSOR_H__ */