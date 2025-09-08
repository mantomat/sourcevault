# SourceVault `gui-qt6`

This directory contains the implementation of the Qt6 implementation of SourceVault.

## Architectural overview

This implementation is component-oriented and uses the Model-View-Controller pattern.

The Model is represented by the `sourcevaultcore` library. Occasionally, you will see some `XXXWrapper` class, where `XXX` is a `core` component. This is because Qt has a strong idea of what a `Model` and a `View` are inside its framework, hence we use these wrappers from `core` components to Qt-compatible `Model`s.

The Controllers consist of a bridge between the Model and the View. They retrieve data from the Model and provide them to the View to display, and use inputs from the View to operate actions on the Model.

The View is the final part. They only consist of a graphical interface to display some data. They don't know anything about the Model logic. They only display some given data, and notify the controller about any events that happen.


## Directory structure

Here is a brief illustration of the directory structure.

- `main.cpp` Creates the app and exectues it.

- `AppController.h/.cpp` Has ownership of the view and lower-order controllers, it creates them and wires everything up.

- `MainWindow.h/.cpp` Is the root view window.

- `pages/` Directory that contains standalone components that act as pages, and their sub-components. Pages are different from `component`s because they are not as reusable.

- `components/` Directory that contains small, reusable components spread throughout the pages.
