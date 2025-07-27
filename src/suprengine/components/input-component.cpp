#include "input-component.h"

using namespace suprengine;

InputComponent::InputComponent( const InputContext& context )
	: _context( context )
{
}

const InputContext& InputComponent::get_input_context() const
{
	return _context;
}
