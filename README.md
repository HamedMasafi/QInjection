# Qt dependency injection lib

QInjection is a dependency injection framework for Qt. It provides a way to automatically inject dependencies into classes, simplifying the process of creating and testing code.

  - QInjection is a dependency injection framework for Qt.
  - It's easy to use and has a simple API.
  - It's designed to be as lightweight as possible.
  - It's easy to use and can be used in any Qt project.
  - Add objects to the pool and they will be automatically injected into your code.

## Quick start
Imagine we have two class named _MyClass1_ and _MyClass2_ that we use them regularly in our app.

There are fre ways to do that:

1. Create them everywhere needed!
2. Create an instance and pass trough object creation and function calls to different levels!
3. Use an _Dependency Injection_

This repo that you are visiting is a simple way for thirth solution
 
This library have a mechanism to store an object into _Pool_ and retrive it wherever needed. Let's look to a simple example:

First: adding objects to dependency pool. In the main method we are adding an instance of _MyClass1_ and _MyClass2_  to the _Pool_. 
```cpp
#include "myclass.h"
#include "dependencypool.h"

#include <QCoreApplication>

MyClass *createMyClass()
{
	auto o = new MyClass;
	// o->setSomeProperty(someValue);
	return o;
}

void foo(MyClass *object = QInjection::Inject)
{
	// do something with object
}

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	QInjection::addSingleton(createMyClass);

	foo(); // <- Empty arguments; the object will be fetched from dependency injection pool
	return a.exec();
}
```

As you can see the object parametere of method foo is not set; it will be fetched from the pool. As the MyClass registration type is singleton, so it will be same in all of the application code.

## Api doc

### Singleton vs Scopped

Singleton objects are same for all of application lifetime, scopped objects are created on every request.

### Add an object to pool
```cpp
template<class T>
void addSingleton();

template<class T>
void addSingleton(T *object);

template<class T>
void addSingleton(T*(*slot)() );

template<class _Owner, class T>
void addSingleton(_Owner *owner, void (_Owner::*slot)(T *));

template<class T>
void addScopped(T *(*slot)())

template<class _Owner, class T>
void addScopped(_Owner *owner, void (_Owner::*slot)(T *));
```

Examples:
```cpp

MyAnotherClass *createMyAnotherClass()
{
    auto o = new MyAnotherClass;
    // set some properties for object o
    return o;
}

int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);

    auto obj = new MyClass;

    QInjection::addSingleton(obj); // pass object to dependency pool
    QInjection::addSingleton(createMyAnotherClass); // pass function pointer to dependency pool

    return a.exec();
}
	
```

### Get object
**There are some method to getting object from the pool**

The _QInjection::Pointer_ class is main method of fetching object from the pool. It work like _QPointer_. It will be deleted it's content if registration type is _scopped_. 

```cpp 
void myMethod2()
{
	QInjection::Pointer<Myclass> object;
	// This is where the object will be taken from the pool. 
	// If it's registration type is of a scopped type, it'll be deleted after the end.
}
```

Another method is using _QInjection::Inject_. Efficient if used in function parametere. In the example below you may pass object of _MyClass_ type, if not it will be taken from dependency injection pool.
```cpp 
void myMethod(Myclass *object = QInjection::Inject)
{
	// You can pass a object of type MyClass to this method. If you don't, the object will be taken from QInjection pool
}
```

### Object change notify

Some times we need to check when an object is adding or removing to _DependencyInjection_ class. for that we have _registerObjectNotify_ method on this class:

```cpp
QInjection->add<Interface>();

...

QInjection->registerObjectNotify(this, &MainWindow::interface_changed);
```
And in header we have a slot for this:

```cpp
public slots:
	void interface_changed(Interface *project);
```
So when an object of type _Interface_ is going to add or remove to DependencyInjection this slot will be called. Note that when object removed from DependencyInjection the parametere of this method will be _null_ptr_

In other face, this method can take a lambda:

```cpp
dep->registerObjectNotify<Interface>(this, [this](interface *interface) {
	if (interface)
		qDebug() << "Interface removved from pool";
	else
		qDebug() << "Interface added to pool";
});
```
