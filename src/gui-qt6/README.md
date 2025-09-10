# SourceVault `gui-qt6`

This directory contains the implementation of the Qt6 implementation of SourceVault.

## Architectural overview

This implementation is component-oriented and uses the Model-View-Presenter pattern.

The Model is represented by the `sourcevaultcore` library. It is a standalone, reusable backend library that knows nothing about the GUI.

The Presenters consist of a bridge between the Model and the View. They retrieve data from the Model and provide them to the View to display, and use inputs from the View to operate actions on the Model and update the View itself.

The View is the final part. They only consist of a graphical interface to display some data. They don't know anything about the Model logic. They only display some given data, and notify the controller about any events that happen. We also use a touch of the MVVM framework - Model data is not served directly to Views, but through custom structs to make the views as "dumb" as possible.

For more information, see [this article](https://tms-outsource.com/blog/posts/mvc-vs-mvvm-vs-mvp/). Note that we are following the MVP pattern, not the MVC as it is presented there, but we use the term `Controller` instead of `Presenter` as it feels more natural.

## Directory structure

Here is a brief illustration of the directory structure.

- `main.cpp` Creates the app and exectues it.

- `AppController.h/.cpp` Has ownership of the view and lower-order controllers, it creates them and wires everything up.

- `MainWindow.h/.cpp` Is the root view window.

- `components/` Directory that contains all the app components.
