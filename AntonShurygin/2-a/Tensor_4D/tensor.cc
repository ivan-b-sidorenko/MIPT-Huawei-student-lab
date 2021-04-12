#include "tensor.hh"

namespace Linear_space
{
    Tensor_4d::Tensor_4d(uint height, uint widht, uint n_chnls, uint n_butch) :  height_(height),
                                                                                 widht_(widht),
                                                                                 n_chnls_(n_chnls),
                                                                                 n_butch_(n_butch)
    {
        //butchs.reserve(n_butch_);
        for (size_t k = 0; k < n_butch_; ++k)
            butchs.emplace_back(height_, widht_, n_chnls);
    }

    Tensor_4d::Tensor_4d(const std::vector<Channel>& chnl_vec, uint height, uint widht, uint n_chnls) : height_(height),
                                                                                                        widht_(widht),
                                                                                                        n_chnls_(n_chnls),
                                                                                                        n_butch_(chnl_vec.size())
    {
        butchs.reserve(n_butch_);

        for (size_t k = 0; k < n_butch_; ++k)
            butchs.emplace_back(chnl_vec[k]);
    }

    Tensor_4d::Tensor_4d(uint height, uint widht, uint n_chnls, uint n_butch, float data) : height_(height),
                                                                                            widht_(widht),
                                                                                            n_chnls_(n_chnls),
                                                                                            n_butch_(n_butch)
    {
        butchs.reserve(n_butch_);

        for (size_t k = 0; k < n_butch_; ++k)
            butchs.emplace_back(height_, widht_, n_chnls, data);
    }

    Tensor_4d::Tensor_4d(uint height, uint widht, uint n_chnls, uint n_butch, enum Mode mode) :   height_(height),
                                                                                                        widht_(widht),
                                                                                                        n_chnls_(n_chnls),
                                                                                                        n_butch_(n_butch)
    {
        butchs.reserve(n_butch_);

        for (size_t k = 0; k < n_butch_; ++k)
            butchs.emplace_back(height_, widht_, n_chnls, mode);
    }



    Tensor_4d::Tensor_4d(const Tensor_4d& tensor)
    {
        height_ = tensor.get_height();
        widht_ = tensor.get_widht();
        n_chnls_ = tensor.get_n_chnls();
        n_butch_ = tensor.get_n_butch();

        for (auto&& butchs_: tensor.butchs)
            butchs.push_back(butchs_);
    }

    Tensor_4d& Tensor_4d::operator= (const Tensor_4d& rhs)
    {
        height_ = rhs.get_height();
        widht_ = rhs.get_widht();
        n_chnls_ = rhs.get_n_chnls();
        n_butch_ = rhs.get_n_butch();

        if (butchs.size() != 0)
            butchs.clear();
    

        for (auto&& butch: rhs.butchs)
            butchs.push_back(butch);
        
        return *this;
    }


    bool Tensor_4d::operator== (const Tensor_4d& rhs)
    {
        height_ = rhs.get_height();
        widht_ = rhs.get_widht();
        n_chnls_ = rhs.get_n_chnls();

        if (rhs.butchs.size() != butchs.size())
            return false;

        for (int k = 0; k < n_chnls_; ++k)
        {
            if (butchs[k] == rhs.butchs[k])
                continue;
            
            else
                return false;
        }
        
        return true;
    }

    uint Tensor_4d::get_height() const
    { 
        return height_;
    }

    uint Tensor_4d::get_widht()  const 
    {
        return widht_;
    }  

    uint Tensor_4d::get_n_chnls() const
    {
        return n_chnls_;
    }

    uint Tensor_4d::get_n_butch() const
    {
        return n_butch_;
    }

    const Channel& Tensor_4d::get_chnl(size_t index)
    {
        return butchs[index];
    }


    Channel& Tensor_4d::operator[] (size_t index)
    {
        return butchs[index];
    }

    std::ostream& operator << (std::ostream& os, Tensor_4d& tensor)
    {
        size_t n_chnl = tensor.get_n_chnls(),
               height = tensor.get_height(),
               widht = tensor.get_widht(),
               n_butch = tensor.get_n_butch();

        for (size_t k = 0; k < n_butch; ++k)
        {
            std::cout << "Butch №" << k + 1 << std::endl;
            std::cout << "______________________________\n";
            Channel tmp = tensor.get_chnl(k);

            os << tmp;
            os << "         " << std::endl;
        }
    }                                                                                                        
}