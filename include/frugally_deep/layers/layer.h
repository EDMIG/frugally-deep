// Copyright 2016, Tobias Hermann.
// https://github.com/Dobiasd/frugally-deep
// Distributed under the MIT License.
// (See accompanying LICENSE file or at
//  https://opensource.org/licenses/MIT)

#pragma once

#include "frugally_deep/typedefs.h"

#include "frugally_deep/matrix3d.h"

#include <cstddef>
#include <memory>

namespace fd
{

class layer;
typedef std::shared_ptr<layer> layer_ptr;
typedef std::vector<layer_ptr> layer_ptrs;

class node;
using node_ptr = std::shared_ptr<node>;
using node_ptrs = std::vector<node_ptr>;

class activation_layer;
typedef std::shared_ptr<activation_layer> activation_layer_ptr;
matrix3ds apply_activation_layer(const activation_layer_ptr& ptr, const matrix3ds& input);

class layer
{
public:
    explicit layer(const std::string& name)
        : name_(name), activation_(nullptr)
    {
    }
    void set_activation(const activation_layer_ptr& activation)
    {
        activation_ = activation;
    }
    virtual ~layer()
    {
    }
    virtual matrix3ds apply(const matrix3ds& input) const final
    {
        const auto result = apply_impl(input);
        if (activation_ == nullptr)
            return result;
        else
            return apply_activation_layer(activation_, result);
    }
    // todo: remove
    virtual const std::string& name() const final
    {
        return name_;
    }

    virtual const node_ptrs get_possible_output_nodes() const
    {
        return {};
    }

    std::string name_;

protected:
    virtual matrix3ds apply_impl(const matrix3ds& input) const = 0;
    activation_layer_ptr activation_;
};

inline layer_ptr get_layer(const layer_ptrs& layers,
    const std::string& layer_id)
{
    const auto is_matching_layer = [layer_id](const layer_ptr& ptr) -> bool
    {
        return ptr->name_ == layer_id;
    };
    return fplus::throw_on_nothing(
        fd::error("layer reference"),
        fplus::find_first_by(is_matching_layer, layers));
}

} // namespace fd
