# Qt dependency injection lib

QInjection is a dependency injection framework for Qt. It provides a way to automatically inject dependencies into classes, simplifying the process of creating and testing code.

  - QInjection is a dependency injection framework for Qt.
  - It has a simple and easy-to-use API.
  - It is lightweight and does not add much overhead to your code.
  - It can be used in any Qt project, regardless of the architecture or design pattern.
  - You just need to add objects to the pool and they will be automatically injected into your code.

## Quick start
Imagine we have two classes named _MyClass1_ and _MyClass2_ that we use them regularly in our app.

There are few ways to do that:

1. Create them everywhere needed!
2. Create an instance and pass it through object creation and function calls to different levels!
3. Use an _Dependency Injection_ framework.

This repo that you are visiting is a simple way for the third solution
 
This library has a mechanism to store an object into a pool and retrieve it wherever needed. Let's take a look at a simple example:

First: adding objects to the dependency pool. In the main method we are adding an instance of _MyClass1_ and _MyClass2_ to the _Pool_. 
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

As you can see the object parameter of the method foo is not set; it will be fetched from the pool. Since MyClass is registered as a singleton, it will be the same object throughout the application code..

## Api doc

### Singleton vs Scopped

Singleton objects are the same for entire application lifetime, scopped objects are created on every request.

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
**There are some methods to get an object from the pool**

The _QInjection::Pointer_ class is the main method of fetching object from the pool. It works like _QPointer_. It will delete it's content if the registration type is _scopped_. 

```cpp 
void myMethod2()
{
	QInjection::Pointer<Myclass> object;
	// This is where the object will be taken from the pool. 
	// If it's registration type is of a scopped type, it'll be deleted after the end.
}
```

Another method is using _QInjection::Inject_. It is Efficient if used in function parameters. In the example below you may pass an object of _MyClass_ type, if not it will be taken from the dependency injection pool.
```cpp 
void myMethod(Myclass *object = QInjection::Inject)
{
	// You can pass a object of type MyClass to this method. If you don't, the object will be taken from QInjection pool
}
```

### Object change notify

Sometimes we need to check when an object is being added or removed from the _DependencyInjection_ class. for that purpose, we have the _registerObjectNotify_ method in this class:

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
So when an object of type _Interface_ is going to be added or removed from DependencyInjection, this slot will be called. Note that when object gets removed from DependencyInjection, the parameter of this method will be _null_ptr_

Alternatively, this method can take a lambda:

```cpp
dep->registerObjectNotify<Interface>(this, [this](interface *interface) {
	if (interface)
		qDebug() << "Interface removved from pool";
	else
		qDebug() << "Interface added to pool";
});
```
