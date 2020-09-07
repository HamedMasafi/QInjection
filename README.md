# Qt dependency injection lib

## Add an object to pool
__Method 1:__ Insert object
```
auto obj = new MyClass;
DependencyInjection::instance()->add(obj);
```
__Method 2:__ Insert object with givin key
```
auto obj = new MyClass;
DependencyInjection::instance()->add(obj, "MyCustomKey");
```

__Method 3:__ Insert with auto create
```
auto obj = DependencyInjection::instance()->add<MyClass>();
```

## Get object
```
auto obj = DependencyInjection::instance()->get<MyClass>();
auto obj2 = sobject_cast<MyClass*>(DependencyInjection::instance()->get("MyCustomKey"));
```

## Remove object
```
DependencyInjection::instance()->remove<MyClass>();
DependencyInjection::instance()->remove("MyCustomKey");
```

## Auto inject
### Injecting from constructor
Imagine that we are three classes named C1, C2 and P. In constructor of P take two parameteres of types C1 and C2. In this case we can write below code. The head 4 lines come into begin of program (line main method) and the last line create a class of type P and take C1 and C2 from dependency pool (the _DependencyInjection_ class) and pass them to cunstructor of P.
```
auto c1 = new C1();
auto c2 = new C2();
dep->add(c1);
dep->add(c2);
...
auto pp = DependencyInjection::instance()->create<P, C1, C2>();
```

### Declare time inject
You can use DependencyPointer template class to automatically getting dependency from pool. In example below that show a class named _P_ have two dependency types of C1 and C2. When this class is going to created this two object take from DependencyInjection and will be initalized without any extra code. 

Note that _DependencyPointer_ class is very like _QPointer_ class. So working with __c1_ and __c2_ is simple as putting a -> right after it, just like normal pointers.

```
class P {
    DependencyPointer<C1> _c1;
    DependencyPointer<C2> _c2;
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
auto obj = DependencyInjection::instance()->add<MyClass>();
auto obj = dep->add<MyClass>();
```

And the macro di_new is equal to _DependencyInjection::instance()->create_, so this two lines are equal:
```
auto pp = DependencyInjection::instance()->create<P, C1, C2>();
auto pp = di_new(P, C1, C2);
```