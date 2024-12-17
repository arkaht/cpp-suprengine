#include "unit-test-event.h"

#include <suprengine/event.h>
#include <suprengine/assert.hpp>

using namespace test;
using namespace suprengine;

struct TestSubject
{
	const char* name;
	int expected_input = -1;

	void listener2( int input )
	{
		ASSERT( input == expected_input );
		printf( "UnitTest: TestSubject::listener2 %s called with %d\n", name, input );
	}
};

static void unit_test_event_listener0( int input )
{
	ASSERT( input == 1 );

	printf( "UnitTest: unit_test_event_listener0 called with %d\n", input );
}

void UnitTestEvent::run()
{
	Event<int> test_event {};

	//	Check that the event is empty at first.
	ASSERT( test_event.get_listeners_count() == 0 );

	//	Check that getters return expected values.
	ASSERT( test_event.is_listening( &unit_test_event_listener0 ) == false );
	ASSERT( test_event.is_listening( &UnitTestEvent::_listener1, this ) == false );

	//	Check that it is possible to bind free functions.
	ASSERT( test_event.listen( &unit_test_event_listener0 ) );
	ASSERT( test_event.get_listeners_count() == 1 );

	//	Check that it isn't possible to duplicate bindings of free functions.
	ASSERT( test_event.listen( &unit_test_event_listener0 ) == false );
	ASSERT( test_event.get_listeners_count() == 1 );

	//	Check that it is possible to bind class methods.
	ASSERT( test_event.listen( &UnitTestEvent::_listener1, this ) );
	ASSERT( test_event.get_listeners_count() == 2 );

	//	Check that getters return expected values.
	ASSERT( test_event.is_listening( &unit_test_event_listener0 ) );
	ASSERT( test_event.is_listening( &UnitTestEvent::_listener1, this ) );

	//	First event call.
	test_event.invoke( 1 );

	//	Check that is it possible to un-bind a free function.
	ASSERT( test_event.unlisten( &unit_test_event_listener0 ) );
	ASSERT( test_event.get_listeners_count() == 1 );

	//	Check that is it possible to bind a lambda.
	ASSERT(
		test_event.listen(
			[&]( int input )
			{
				ASSERT( input == 2 || input == 3 );
				printf( "UnitTest: lambda called with %d\n", input );
			} ),
		"UnitTestEvent"
	);
	ASSERT( test_event.get_listeners_count() == 2 );

	//	Check that it is possible to bind the same class method for different objects.
	TestSubject madison { "Madison", 2 };
	TestSubject charles { "Charles", 2 };
	ASSERT( test_event.listen( &TestSubject::listener2, &madison ) );
	ASSERT( test_event.listen( &TestSubject::listener2, &charles ) );
	ASSERT( test_event.get_listeners_count() == 4 );

	//	Check that it isn't possible to duplicate bindings of class methods with the same objects.
	ASSERT( test_event.listen( &TestSubject::listener2, &madison ) == false );
	ASSERT( test_event.listen( &TestSubject::listener2, &charles ) == false );
	ASSERT( test_event.get_listeners_count() == 4 );

	//	Check that getters return expected values.
	ASSERT( test_event.is_listening( &TestSubject::listener2, &madison ) );
	ASSERT( test_event.is_listening( &TestSubject::listener2, &charles ) );

	//	Second event call.
	test_event.invoke( 2 );

	//	Check that is possible to un-bind class methods.
	ASSERT( test_event.unlisten( &UnitTestEvent::_listener1, this ) );
	ASSERT( test_event.unlisten( &TestSubject::listener2, &charles ) );
	ASSERT( test_event.get_listeners_count() == 2 );

	//	Check that getters return expected values.
	ASSERT( test_event.is_listening( &UnitTestEvent::_listener1, this ) == false );
	ASSERT( test_event.is_listening( &TestSubject::listener2, &charles ) == false );

	madison.expected_input = 3;

	//	Third event call.
	test_event.invoke( 3 );
}

void UnitTestEvent::_listener1( int input )
{
	ASSERT( input == 1 || input == 2 );

	printf( "UnitTest: UnitTestEvent::_listener1 called with %d\n", input );
}
