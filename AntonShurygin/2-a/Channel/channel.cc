#include "channel.hh"

namespace Linear_space
{
    Channel::Channel(uint height, uint widht, uint n_chnls) : height_(height),
                                                              widht_(widht),
                                                              n_chnls_(n_chnls)
    {
        //channels.reserve(n_chnls_);

        for (size_t k = 0; k < n_chnls_; ++k)
            channels.emplace_back(height_, widht_, 0);

    }

    Channel::Channel(const std::vector<Matrix>& matr_vec, uint height, uint widht) : height_(height),
                                                                                       widht_(widht),
                                                                                       n_chnls_(matr_vec.size())
    {
        //channels.reserve(n_chnls_);
        for (size_t k = 0; k < n_chnls_; ++k)
            channels.emplace_back(matr_vec[k]);
    }

    Channel::Channel(uint height, uint widht, uint n_chnls, float data) : height_(height),
                                                                          widht_(widht),
                                                                          n_chnls_(n_chnls)
    {
        //channels.reserve(n_chnls_);
        for (size_t k = 0; k < n_chnls_; ++k)
            channels.emplace_back(height_, widht_, data);
    }

    Channel::Channel(uint height, uint widht, uint n_chnls, enum Mode mode) : height_(height),
                                                                                    widht_(widht),
                                                                                    n_chnls_(n_chnls)
    {
        //channels.reserve(n_chnls_);
        for (size_t k = 0; k < n_chnls_; ++k)
            channels.emplace_back(height_, widht_, mode);
    }

    Channel::Channel(const Channel& chnl)
    {
        height_ = chnl.get_height();
        widht_ = chnl.get_widht();
        n_chnls_ = chnl.get_n_chnls();

        for (auto&& chnl_: chnl.channels)
            channels.push_back(chnl_);
    }

    Channel& Channel::operator= (const Channel& rhs)
    {
        height_ = rhs.get_height();
        widht_ = rhs.get_widht();
        n_chnls_ = rhs.get_n_chnls();

        if (channels.size() != 0)
            channels.clear();
    

        for (auto&& chnl: rhs.channels)
            channels.push_back(chnl);
        
        return *this;
    }


    bool Channel::operator== (const Channel& rhs)
    {
        height_ = rhs.get_height();
        widht_ = rhs.get_widht();
        n_chnls_ = rhs.get_n_chnls();

        if (rhs.channels.size() != channels.size())
            return false;

        for (int k = 0; k < n_chnls_; ++k)
        {
            if (channels[k] == rhs.channels[k])
                continue;
            
            else
                return false;
        }
        
        return true;
    }

    uint Channel::get_height() const
    { 
        return height_;
    }

    uint Channel::get_widht()  const 
    {
        return widht_;
    }  

    uint Channel::get_n_chnls() const
    {
        return n_chnls_;
    }

    const Matrix& Channel::get_matr(size_t index)
    {
        return channels[index];
    }


    Matrix& Channel::operator[] (size_t index)
    {
        return channels[index];
    }

    std::ostream& operator << (std::ostream& os, Channel& chnl)
    {
    size_t n_chnl = chnl.get_n_chnls(),
           height = chnl.get_height(),
           widht = chnl.get_widht();

    for (size_t k = 0; k < n_chnl; ++k)
    {
        std::cout << "Layer №" << k + 1 << std::endl;
        Matrix tmp = chnl[k];

        os << tmp;
        os << "         " << std::endl;
    }
}                                                                                                     
}