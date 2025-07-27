#pragma once

#include "suprengine/core/component.h"
#include "suprengine/input/input-action.h"

namespace suprengine
{
    class InputComponent : public Component
    {
    public:
    	explicit InputComponent( const InputContext& context );

    	template <typename T>
    	T read_value( const InputAction<T>* action ) const
    	{
    		if ( action == nullptr ) return T();
    		return action->read_value( _context );
    	}

		const InputContext& get_input_context() const;

    private:
	    InputContext _context {};
    };
}
