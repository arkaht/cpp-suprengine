#include "unit-test-event.h"

#include <suprengine/utils/event.h>
#include <suprengine/utils/assert.h>

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

	printf(
		"UnitTest: test_event internal listeners vector is estimated to cost %zd bytes for a capacity of %zd elements\n",
		test_event.get_listeners_memory_usage(),
		test_event.get_listeners_capacity()
	);

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
	const char* lambda_name = "VeryCoolLambda";
	ASSERT(
		test_event.listen(
			[&]( int input )
			{
				ASSERT( input == 2 || input == 3 || input == 4 );
				printf( "UnitTest: lambda \"%s\" called with %d\n", lambda_name, input );
			}
		)
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

	//	Remove all listeners so only the lambda remains.
	ASSERT( test_event.unlisten( &TestSubject::listener2, &madison ) );
	ASSERT( test_event.get_listeners_count() == 1 );

	//	Estimate memory usage of the event class.
	const size_t instance_memory = sizeof( test_event );
	const size_t listeners_memory = test_event.get_listeners_memory_usage();
	printf(
		"UnitTest: test_event is estimated to cost %zd (%zd + %zd) bytes for a capacity of %zd elements\n",
		instance_memory + listeners_memory,
		instance_memory, listeners_memory,
		test_event.get_listeners_capacity()
	);

	//	Check it is possible to bind multiple lambdas without them collapsing in the implementation.
	constexpr int LAMBDAS_COUNT = 32;
	for ( int i = 0; i < LAMBDAS_COUNT; i++ )
	{
		ASSERT(
			test_event.listen(
				[i]( int input )
				{
					ASSERT( input == 4 );
					printf( "UnitTest: lambda %d called with %d\n", i, input );
				}
			)
		);
	}
	ASSERT( test_event.get_listeners_count() == 1 + LAMBDAS_COUNT );

	test_event.invoke( 4 );

	printf( "UnitTest: All Passed!\n" );
}

void UnitTestEvent::_listener1( int input )
{
	ASSERT( input == 1 || input == 2 );

	printf( "UnitTest: UnitTestEvent::_listener1 called with %d\n", input );
}
