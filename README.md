# Qt dependency injection lib

QInjection is a dependency injection framework for Qt. It provides a way to automatically inject dependencies into classes, simplifying the process of creating and testing code.

## Quick start
Imagine we have two class named _MyClass1_ and _MyClass2_ that we use them regularly in our app.

There are fre ways to do that:

1. Create them everywhere needed!
2. Create an instance and pass trough object creation and function calls to different levels!
3. Use an _Dependency Injection_

This repo that you are visiting is a simple way for thirth solution
 
This library have a mechanism to store an object into _Pool_ and retrive it wherever needed. Let's look to a simple example:

First: adding objects to dependency pool. In the main method we are adding an instance of _MyClass1_ and _MyClass2_  to the _Pool_. 
```
#include "myclass1.h"
#include "myclass2.h"
#include "dependencyinjection.h"

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    dep->add<MyClass1>();
    dep->add<MyClass2>();

    return a.exec();
}
```

And in the other class 
```
#ifndef P_H
#define P_H

#include <QObject>
#include "dependencypointer.h"

class MyClass1;
class C2;
class P : public QObject
{
    Q_OBJECT
    QInjection::Pointer<MyClass1> _class1;

public:
    explicit P(QObject *parent = nullptr);
    void foo(MyClass2 *c2 = Dependency::Inject());

signals:

};

#endif // P_H
```
As you can see the class _P_ need an object of type _MyClass1_ and with _Dependency::Pointer_ automatically it will get from _Pool_. In the method _foo_ the class need an object of type _MyClass2_ and with _Dependency::Inject_ utility this object will be injected to this class without custom code needle.

## Api doc
#### Singleton vs Scopped

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

**Method 1:** Insert object
```cpp
auto obj = new MyClass;
DependencyInjection::instance()->add(obj);
```

**Method 2:** Insert object with givin key
```
auto obj = new MyClass;
Dependency::Pool::instance()->add(obj, "MyCustomKey");
```


**Method 3:** Insert with auto create
```
auto obj = Dependency::Pool::instance()->add<MyClass>();
```

### Get object
#### There are some method to getting object from the pool

```cpp 
void myMethod(Myclass *object = QInjection::Inject)
{
	// You can pass a object of type MyClass to this method. If you don't, the object will be taken from QInjection pool
}
```

```cpp 
void myMethod2()
{
	QInjection::Pointer<Myclass> object;
	// This is where the object will be taken from the pool. 
	// If it's registration type is of a scopped type, it'll be deleted after the end.
}
```



```
auto obj = Dependency::Pool::instance()->get<MyClass>();
auto obj2 = sobject_cast<MyClass*>(Dependency::Pool::instance()->get("MyCustomKey"));
```

### Remove object
```
DependencyInjection::instance()->remove<MyClass>();
DependencyInjection::instance()->remove("MyCustomKey");
```

### Auto inject
### Injecting from constructor
Imagine that we are three classes named C1, C2 and P. In constructor of P take two parameteres of types C1 and C2. In this case we can write below code. The head 4 lines come into begin of program (line main method) and the last line create a class of type P and take C1 and C2 from dependency pool (the _DependencyInjection_ class) and pass them to cunstructor of P.
```
auto c1 = new C1();
auto c2 = new C2();
dep->add(c1);
dep->add(c2);
...
auto pp = Dependency::Pool::instance()->create<P, C1, C2>();
```

### Declare time inject
You can use DependencyPointer template class to automatically getting dependency from pool. In example below that show a class named _P_ have two dependency types of C1 and C2. When this class is going to created this two object take from DependencyInjection and will be initalized without any extra code. 

Note that _DependencyPointer_ class is very like _QPointer_ class. So working with __c1_ and __c2_ is simple as putting a -> right after it, just like normal pointers.

```
class P {
    Dependency::Pointer<C1> _c1;
    Dependency::Pointer<C2> _c2;
public:
    ...    
};
```

## Object change notify

Some times we need to check when an object is adding or removing to _DependencyInjection_ class. for that we have _registerObjectNotify_ method on this class:
```
dep->add<Interface>();
...
dep->registerObjectNotify(this, &MainWindow::interface_changed);
```
And in header we have a slot for this:
```
public slots:
	void interface_changed(Interface *project);
```
So when an object of type _Interface_ is going to add or remove to DependencyInjection this slot will be called. Note that when object removed from DependencyInjection the parametere of this method will be _null_ptr_

In other face, this method can take a lambda:
```
dep->registerObjectNotify<Interface>(this, [this](interface *interface) {
	if (interface)
		qDebug() << "Interface removved from pool";
	else
		qDebug() << "Interface added to pool";
});
```

## Macros
The madro _dep_ is equal to _DependencyInjection::instance()_ so the this two lines are same:
```
auto obj = Dependency::Pool::instance()->add<MyClass>();
auto obj = dep->add<MyClass>();
```

And the macro di_new is equal to _DependencyInjection::instance()->create_, so this two lines are equal:

```
auto pp = DependencyInjection::instance()->create<P, C1, C2>();
auto pp = di_new(P, C1, C2);
```
