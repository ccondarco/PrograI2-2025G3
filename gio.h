#include <iostream>
#include <fstream>
#include <cstring>  
#include <vector>
using namespace std;

// ESTRUCTURAS
struct Fecha {
    int mes;
    int dia;
    int anio;
};

struct DatosCliente {
    char CI_Cliente[10];
    char nombre[30];
    char apellido[30];
    Fecha fechaNacimiento;
    int telefono;
    char correo[30];
    bool eliminado;
};

struct DatosEmpleado {
    char CI_Empleado[10];
    char nombre[30];
    char apellido[30];
    int rango;
    bool eliminado;
};

struct DatosProducto {
    int codigo;
    char modelo[30];
    int categoria;
    float precioVenta;
    int stock;
    bool eliminado;
};

struct ventaProducto {
    char CI_Cliente[10];
    char CI_Empleado[10];
    Fecha fechaDeVenta;
    int codigoProducto;
    int cantidad;
    float precioUnitario;
    bool eliminada;
    bool facturada;
    int numeroFactura;
};

// Vector para categorias de productos 
vector<string> categoriasProductoVector = {"Celular", "Tablet", "Laptop", "Televisor", "Monitor", "Parlante", "Proyector", "Lavadora", "Refrigerador"};

// ==================== FUNCIONES DE GESTION DE PRODUCTOS ====================

void categoriasProductoMenu() {
    cout << "\tCATEGORIAS\n";
    cout << "\t\t1. Celular\n";
    cout << "\t\t2. Tablet\n";
    cout << "\t\t3. Laptop\n";
    cout << "\t\t4. Televisor\n";
    cout << "\t\t5. Monitor\n";
    cout << "\t\t6. Parlante\n";
    cout << "\t\t7. Proyector\n";
    cout << "\t\t8. Lavadora\n";
    cout << "\t\t9. Refrigerador\n";
}

DatosProducto insertarDatosProducto() {
    DatosProducto producto;

    cout << "Codigo: ";
    cin >> producto.codigo;
    cin.ignore();
    cout << "Modelo: ";
    cin.getline(producto.modelo, 30);
    cout << "Categoria: ";
    categoriasProductoMenu();
    cin >> producto.categoria;
    producto.categoria -= 1; // Ajustar para el vector
    cout << "Precio de venta: ";
    cin >> producto.precioVenta;
    cout << "Stock: ";
    cin >> producto.stock;
    producto.eliminado = false;
    
    return producto;
}

void agregarProducto() {
    ofstream archivo;
    DatosProducto producto = insertarDatosProducto();
    archivo.open("productos.bin", ios::binary | ios::app);
    if (archivo.is_open()) {
        archivo.write((char*)(&producto), sizeof(DatosProducto));
        cout << "Producto agregado con exito\n";
    } else {
        cout << "Error al agregar producto\n";
        return;
    }
    archivo.close();
}

void mostrarListaProductosPantalla() {
    ifstream archivo;
    DatosProducto producto;

    archivo.open("productos.bin", ios::binary);
    if (archivo.is_open()) {
        cout << endl << "============ LISTA DE PRODUCTOS DISPONIBLES ===================" << endl;
        while(archivo.read((char*)(&producto), sizeof(DatosProducto))) {
            if (producto.eliminado == false) {
                cout << "\tCodigo: " << producto.codigo << endl;
                cout << "\tModelo: " << producto.modelo << endl;
                cout << "\tCategoria: " << categoriasProductoVector[producto.categoria] << endl;
                cout << "\tPrecio de venta: " << producto.precioVenta << endl;
                cout << "\tStock: " << producto.stock << endl;
                cout << "--------------------------------------------------------" << endl;
            }
        }
        archivo.close();

    } else {
        cout << "Error al abrir el archivo de productos o no hay productos registrados" << endl;
        return;
    }
}

void ModificarProducto() {
    fstream archivo;
    DatosProducto producto;
    int codigoBuscado;
    bool encontrado = false;
    char confirmacion;

    cout << "Ingrese el codigo del producto a modificar: ";
    cin >> codigoBuscado;

    archivo.open("productos.bin", ios::in | ios::out | ios::binary);
    if (archivo.is_open()) {
        while (archivo.read((char*)(&producto), sizeof(DatosProducto))) {
            if (codigoBuscado == producto.codigo && producto.eliminado == false) {
                cout << "=== PRODUCTO A MODIFICAR ===" << endl;
                cout << "\tCodigo: " << producto.codigo << endl;
                cout << "\tModelo: " << producto.modelo << endl;
                cout << "\tCategoria: " << categoriasProductoVector[producto.categoria] << endl;
                cout << "\tPrecio de venta: " << producto.precioVenta << endl;
                cout << "\tStock: " << producto.stock << endl;
                encontrado = true;

                cout << "Desea modificar este producto? (s/n): ";
                cin >> confirmacion;
                cin.ignore();
                if (confirmacion == 's' || confirmacion == 'S') {
                    archivo.seekp(-sizeof(DatosProducto), ios::cur);
                    cout << "NUEVOS DATOS DEL PRODUCTO" << endl;
                    cout << "Modelo: ";
                    cin.getline(producto.modelo, 30);
                    cout << "Categoria: ";
                    categoriasProductoMenu();
                    cin >> producto.categoria;
                    producto.categoria -= 1;
                    cout << "Precio de venta: ";
                    cin >> producto.precioVenta;
                    cout << "Stock: ";
                    cin >> producto.stock;
                    archivo.write((char*)(&producto), sizeof(DatosProducto));
                    cout << "Producto modificado con exito" << endl;
                }
                break;
            }
        }
        if (encontrado == false) {
            cout << "No se encontro el producto o esta eliminado." << endl;
        }
    } else {
        cout << "Error al modificar producto" << endl;
    }
    archivo.close();
}

void eliminarProducto() {
    fstream archivo;
    DatosProducto producto;
    int codigoBuscado;
    bool encontrado = false;
    char confirmacion;

    cout << "Ingrese el codigo del producto a eliminar: ";
    cin >> codigoBuscado;

    archivo.open("productos.bin", ios::in | ios::out | ios::binary);
    if (archivo.is_open()) {
        while (archivo.read((char*)(&producto), sizeof(DatosProducto))) {
            if (codigoBuscado == producto.codigo && producto.eliminado == false) {
                cout << "=== PRODUCTO A ELIMINAR ===" << endl;
                cout << "\tCodigo: " << producto.codigo << endl;
                cout << "\tModelo: " << producto.modelo << endl;
                cout << "\tCategoria: " << categoriasProductoVector[producto.categoria] << endl;
                cout << "\tPrecio de venta: " << producto.precioVenta << endl;
                cout << "\tStock: " << producto.stock << endl;
                encontrado = true;

                cout << "Desea eliminar este producto? (s/n): ";
                cin >> confirmacion;
                if (confirmacion == 's' || confirmacion == 'S') {
                    archivo.seekp(-sizeof(DatosProducto), ios::cur);
                    producto.eliminado = true;
                    archivo.write((char*)(&producto), sizeof(DatosProducto));
                    cout << "Producto eliminado con exito" << endl;
                }
                break;
            }
        }
        if (encontrado == false) {
            cout << "No se encontro el producto." << endl;
        }
    } else {
        cout << "Error al eliminar producto" << endl;
    }
    archivo.close();
}

void buscarProductoPorCodigo() {
    ifstream archivo;
    DatosProducto producto;
    bool encontrado = false;
    int codigoBuscado;

    cout << "Ingrese el codigo del producto que busca: ";
    cin >> codigoBuscado;

    archivo.open("productos.bin", ios::binary);
    if (archivo.is_open()) {
        while (archivo.read((char*)(&producto), sizeof(DatosProducto))) {
            if (producto.codigo == codigoBuscado && producto.eliminado == false) {
                cout << "== PRODUCTO ENCONTRADO ==" << endl;
                cout << "Codigo: " << producto.codigo << endl;
                cout << "Modelo: " << producto.modelo << endl;
                cout << "Categoria: " << categoriasProductoVector[producto.categoria] << endl;
                cout << "Precio: " << producto.precioVenta << endl;
                cout << "Stock: " << producto.stock << endl;
                cout << "=========================" << endl;
                encontrado = true;
                break;
            }
        }
        if (encontrado == false) {
            cout << "No se encontro un producto con ese codigo o esta eliminado." << endl;
        }
        archivo.close();
    } else {
        cout << "Error al buscar el producto" << endl;
        return;
    }
}

void productosBajoStock() {
    ifstream archivo;
    DatosProducto producto;

    archivo.open("productos.bin", ios::binary);
    if (archivo.is_open()) {
        cout << "====== PRODUCTOS DE BAJO STOCK (menos de 5 unidades) ======" << endl;
        while (archivo.read((char*)(&producto), sizeof(DatosProducto))) {
            if (producto.stock < 5 && producto.eliminado == false) {
                cout << "Codigo: " << producto.codigo << endl;
                cout << "Modelo: " << producto.modelo << endl;
                cout << "Categoria: " << categoriasProductoVector[producto.categoria] << endl;
                cout << "Precio: " << producto.precioVenta << endl;
                cout << "Stock: " << producto.stock << endl;
                cout << "=========================" << endl;
            }
        }
        archivo.close();
    } else {
        cout << "Error al buscar productos" << endl;
        return;
    }
}

void menuProductos() {
    int opcion;
    do {
        cout << endl << "== MENU PRODUCTOS ==" << endl;
        cout << "\t1. Agregar producto" << endl;
        cout << "\t2. Modificar producto" << endl;
        cout << "\t3. Eliminar producto" << endl;
        cout << "\t4. Mostrar lista de productos disponibles" << endl;
        cout << "\t5. Buscar producto por codigo" << endl;
        cout << "\t6. Reporte: productos con bajo stock (menos de 5 unidades)" << endl;
        cout << "\t0. Volver" << endl;
        cout << "--> ";
        cin >> opcion;
        cin.ignore();
        
        switch(opcion) {
            case 1:
                agregarProducto();
                break;
            case 2:
                ModificarProducto();
                break;
            case 3:
                eliminarProducto();
                break;
            case 4:
                mostrarListaProductosPantalla();
                break;
            case 5:
                buscarProductoPorCodigo();
                break;
            case 6:
                productosBajoStock();
                break;
            case 0:
                cout << "Volviendo al menu principal..." << endl;
                break;
            default:
                cout << "Opcion no valida" << endl;
        }
    } while (opcion != 0);
}

// ==================== FUNCIONES DE GESTION DE CLIENTES Y EMPLEADOS ====================

void llenarDatosCliente(DatosCliente &c) {
    cout << endl << "INGRESO DE DATOS DEL CLIENTE" << endl;
    cout << "Ingrese el CI del cliente: ";
    cin.getline(c.CI_Cliente, 10);
    cout << "Ingrese el nombre del cliente: ";
    cin.getline(c.nombre, 30);
    cout << "Ingrese el apellido del cliente: ";
    cin.getline(c.apellido, 30);
    cout << "Ingrese la fecha de nacimiento del cliente" << endl;
    cout << "Ingrese el Mes: ";
    cin >> c.fechaNacimiento.mes;
    cout << "Ingrese el Dia: ";
    cin >> c.fechaNacimiento.dia;
    cout << "Ingrese el Anio: ";
    cin >> c.fechaNacimiento.anio;
    cout << "Ingrese el telefono del cliente: ";
    cin >> c.telefono;
    cin.ignore();
    cout << "Ingrese el correo del cliente: ";
    cin.getline(c.correo, 30);
    
    c.eliminado = false;
    
    cout << endl;
    
    ofstream archivo;
    archivo.open("clientes.bin", ios::binary | ios::app);
    
    if (!archivo.is_open()) {
        cout << "Error: no se pudo guardar clientes.bin" << endl;
        return;
    }
    
    archivo.write((char*)&c, sizeof(DatosCliente));
    archivo.close();
    cout << "Cliente guardado exitosamente en clientes.bin" << endl;
}

void mostrarDatosCliente(DatosCliente c) {
    cout << endl << "DATOS DEL CLIENTE ACTUAL" << endl;
    cout << "El CI del cliente es: " << c.CI_Cliente << endl;
    cout << "El nombre del cliente es: " << c.nombre << endl;
    cout << "El apellido del cliente es: " << c.apellido << endl;
    cout << "La fecha de nacimiento del cliente es: " << c.fechaNacimiento.mes << "/" << c.fechaNacimiento.dia << "/" << c.fechaNacimiento.anio << endl;
    cout << "El telefono del cliente es: " << c.telefono << endl;
    cout << "El correo del cliente es: " << c.correo << endl;
    cout << "Estado: " << (c.eliminado ? "ELIMINADO" : "ACTIVO") << endl;
}

void llenarDatosEmpleado(DatosEmpleado &e) {
    cout << endl << "INGRESO DE DATOS EMPLEADO" << endl;
    cout << "Ingrese el CI del empleado: ";
    cin.getline(e.CI_Empleado, 10);
    cout << "Ingrese el nombre del empleado: ";
    cin.getline(e.nombre, 30);
    cout << "Ingrese el apellido del empleado: ";
    cin.getline(e.apellido, 30);
    cout << "Ingrese el rango del empleado (numero entero): " << endl;
    cout << "1. Vendedor " << endl;
    cout << "2. Cajero " << endl;
    cout << "Seleccione: ";
    cin >> e.rango;
    cin.ignore();
    
    e.eliminado = false;
    
    ofstream archivo;
    archivo.open("empleado.bin", ios::binary | ios::app);
    
    if (!archivo.is_open()) {
        cout << "Error: no se pudo crear/abrir empleado.bin" << endl;
        return;
    }
    
    archivo.write((char*)&e, sizeof(DatosEmpleado));
    archivo.close();
    cout << "Empleado guardado exitosamente en empleado.bin" << endl;
}

void mostrarDatosEmpleado(DatosEmpleado e) {
    cout << endl << "DATOS EMPLEADO ACTUAL" << endl;
    cout << "El CI del empleado es: " << e.CI_Empleado << endl;
    cout << "El nombre del empleado es: " << e.nombre << endl;
    cout << "El apellido del empleado es: " << e.apellido << endl;
    cout << "El rango del empleado es: " << e.rango << endl;
    cout << "Estado: " << (e.eliminado ? "ELIMINADO" : "ACTIVO") << endl;
}

void mostrarTodosClientes() {
    ifstream archivo;
    DatosCliente c;
    int contador = 0;
    int activos = 0;
    
    archivo.open("clientes.bin", ios::binary);
    
    if (!archivo.is_open()) {
        cout << "No hay clientes registrados o error al abrir el archivo." << endl;
        return;
    }
    
    cout << endl << "=== LISTA DE TODOS LOS CLIENTES ===" << endl;
    
    while (archivo.read((char*)&c, sizeof(DatosCliente))) {
        contador++;
        if (!c.eliminado) {
            activos++;
            cout << endl << "CLIENTE " << activos << ":" << endl;
            cout << "CI: " << c.CI_Cliente << endl;
            cout << "Nombre: " << c.nombre << " " << c.apellido << endl;
            cout << "Fecha Nacimiento: " << c.fechaNacimiento.mes << "/" << c.fechaNacimiento.dia << "/" << c.fechaNacimiento.anio << endl;
            cout << "Telefono: " << c.telefono << endl;
            cout << "Correo: " << c.correo << endl;
            cout << "--------------------------------------" << endl;
        }
    }
    
    archivo.close();
    
    if (contador == 0) {
        cout << "No hay clientes registrados en el archivo." << endl;
    } else {
        cout << endl << "Total de clientes registrados: " << contador << endl;
        cout << "Clientes activos: " << activos << endl;
        cout << "Clientes eliminados (no mostrados): " << (contador - activos) << endl;
    }
}

void eliminarCliente() {
    char ciEliminar[10];
    fstream archivo;
    DatosCliente c;
    bool encontrado = false;
    
    cout << endl << "ELIMINAR CLIENTE (ELIMINACION LOGICA)" << endl;
    cout << "Ingrese el CI del cliente a eliminar: ";
    cin.getline(ciEliminar, 10);
    
    archivo.open("clientes.bin", ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) {
        cout << "No hay clientes registrados para eliminar" << endl;
        return;
    }
    
    while (archivo.read((char*)&c, sizeof(DatosCliente)) && !encontrado) {
        if (strcmp(c.CI_Cliente, ciEliminar) == 0 && !c.eliminado) {
            encontrado = true;
            cout << "Cliente encontrado: " << c.nombre << " " << c.apellido << endl;
            cout << "Esta seguro de marcar como eliminado? (s/n): ";
            char confirmacion;
            cin >> confirmacion;
            cin.ignore();
            
            if (confirmacion == 's' || confirmacion == 'S') {
                c.eliminado = true;
                archivo.seekp(-sizeof(DatosCliente), ios::cur);
                archivo.write((char*)&c, sizeof(DatosCliente));
                archivo.flush();
                cout << "Cliente marcado como ELIMINADO correctamente." << endl;
            } else {
                cout << "Operacion cancelada." << endl;
            }
        }
    }
    
    archivo.close();
    
    if (!encontrado) {
        cout << "No se encontro cliente activo con CI: " << ciEliminar << endl;
    }
}

void modificarCliente() {
    char ciModificar[10];
    fstream archivo;
    DatosCliente c;
    bool encontrado = false;
    int opcion;
    
    cout << endl << "MODIFICAR CLIENTE" << endl;
    cout << "Ingrese el CI del cliente a modificar: ";
    cin.getline(ciModificar, 10);
    
    archivo.open("clientes.bin", ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) {
        cout << "No hay clientes registrados para modificar" << endl;
        return;
    }
    
    while (archivo.read((char*)&c, sizeof(DatosCliente)) && !encontrado) {
        if (strcmp(c.CI_Cliente, ciModificar) == 0 && !c.eliminado) {
            encontrado = true;
            
            cout << endl << "CLIENTE ENCONTRADO:" << endl;
            cout << "CI: " << c.CI_Cliente << endl;
            cout << "Nombre: " << c.nombre << " " << c.apellido << endl;
            cout << "Telefono: " << c.telefono << endl;
            cout << "Correo: " << c.correo << endl;
            
            do {
                cout << endl << "Que dato desea modificar?" << endl;
                cout << "1. Nombre" << endl;
                cout << "2. Apellido" << endl;
                cout << "3. Telefono" << endl;
                cout << "4. Correo electronico" << endl;
                cout << "5. Fecha de nacimiento" << endl;
                cout << "6. Salir de modificacion" << endl;
                cout << "Seleccione una opcion: ";
                cin >> opcion;
                cin.ignore();
                
                switch(opcion) {
                    case 1:
                        cout << "Ingrese el nuevo nombre: ";
                        cin.getline(c.nombre, 30);
                        cout << "Nombre actualizado correctamente." << endl;
                        break;
                    case 2:
                        cout << "Ingrese el nuevo apellido: ";
                        cin.getline(c.apellido, 30);
                        cout << "Apellido actualizado correctamente." << endl;
                        break;
                    case 3:
                        cout << "Ingrese el nuevo telefono: ";
                        cin >> c.telefono;
                        cin.ignore();
                        cout << "Telefono actualizado correctamente." << endl;
                        break;
                    case 4:
                        cout << "Ingrese el nuevo correo electronico: ";
                        cin.getline(c.correo, 30);
                        cout << "Correo electronico actualizado correctamente." << endl;
                        break;
                    case 5:
                        cout << "Ingrese la nueva fecha de nacimiento:" << endl;
                        cout << "Mes: ";
                        cin >> c.fechaNacimiento.mes;
                        cout << "Dia: ";
                        cin >> c.fechaNacimiento.dia;
                        cout << "Anio: ";
                        cin >> c.fechaNacimiento.anio;
                        cin.ignore();
                        cout << "Fecha de nacimiento actualizada correctamente." << endl;
                        break;
                    case 6:
                        cout << "Saliendo de la modificacion..." << endl;
                        break;
                    default:
                        cout << "Opcion no valida. Intente de nuevo." << endl;
                }
                
            } while (opcion != 6);
            
            archivo.seekp(-sizeof(DatosCliente), ios::cur);
            archivo.write((char*)&c, sizeof(DatosCliente));
            archivo.flush();
        }
    }
    
    archivo.close();
    
    if (!encontrado) {
        cout << "No se encontro cliente activo con CI: " << ciModificar << endl;
    } else {
        cout << "Cliente modificado correctamente." << endl;
    }
}

void mostrarTodosEmpleados() {
    ifstream archivo;
    DatosEmpleado e;
    int contador = 0;
    int activos = 0;
    
    archivo.open("empleado.bin", ios::binary);
    
    if (!archivo.is_open()) {
        cout << "No hay empleados registrados o error al abrir el archivo." << endl;
        return;
    }
    
    cout << endl << "=== LISTA DE TODOS LOS EMPLEADOS ===" << endl;
    
    while (archivo.read((char*)&e, sizeof(DatosEmpleado))) {
        contador++;
        if (!e.eliminado) {
            activos++;
            cout << endl << "EMPLEADO " << activos << ":" << endl;
            cout << "CI: " << e.CI_Empleado << endl;
            cout << "Nombre: " << e.nombre << " " << e.apellido << endl;
            cout << "Rango: " << e.rango << " (";
            if (e.rango == 1) {
                cout << "Vendedor";
            }
            else if (e.rango == 2) {
                cout << "Cajero";
            }
            else {
                cout << "Desconocido";
            }
            cout << ")" << endl;
            cout << "--------------------------------------" << endl;
        }
    }
    
    archivo.close();
    
    if (contador == 0) {
        cout << "No hay empleados registrados en el archivo." << endl;
    } else {
        cout << endl << "Total de empleados registrados: " << contador << endl;
        cout << "Empleados activos: " << activos << endl;
        cout << "Empleados eliminados (no mostrados): " << (contador - activos) << endl;
    }
}

void eliminarEmpleado() {
    char ciEliminar[10];
    fstream archivo;
    DatosEmpleado e;
    bool encontrado = false;
    
    cout << endl << "ELIMINAR EMPLEADO " << endl;
    cout << "Ingrese el CI del empleado a eliminar: ";
    cin.getline(ciEliminar, 10);
    
    archivo.open("empleado.bin", ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) {
        cout << "No hay empleados registrados para eliminar." << endl;
        return;
    }
    
    while (archivo.read((char*)&e, sizeof(DatosEmpleado)) && !encontrado) {
        if (strcmp(e.CI_Empleado, ciEliminar) == 0 && !e.eliminado) {
            encontrado = true;
            cout << "Empleado encontrado: " << e.nombre << " " << e.apellido << endl;
            cout << "Esta seguro de marcar como eliminado? (s/n): ";
            char confirmacion;
            cin >> confirmacion;
            cin.ignore();
            
            if (confirmacion == 's' || confirmacion == 'S') {
                e.eliminado = true;
                archivo.seekp(-sizeof(DatosEmpleado), ios::cur);
                archivo.write((char*)&e, sizeof(DatosEmpleado));
                archivo.flush();
                cout << "Empleado marcado como ELIMINADO correctamente." << endl;
            } else {
                cout << "Operacion cancelada." << endl;
            }
        }
    }
    
    archivo.close();
    
    if (!encontrado) {
        cout << "No se encontro empleado activo con CI: " << ciEliminar << endl;
    }
}

void modificarEmpleado() {
    char ciModificar[10];
    fstream archivo;
    DatosEmpleado e;
    bool encontrado = false;
    int opcion;
    
    cout << endl << "MODIFICAR EMPLEADO" << endl;
    cout << "Ingrese el CI del empleado a modificar: ";
    cin.getline(ciModificar, 10);
    
    archivo.open("empleado.bin", ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) {
        cout << "No hay empleados registrados para modificar" << endl;
        return;
    }
    
    while (archivo.read((char*)&e, sizeof(DatosEmpleado)) && !encontrado) {
        if (strcmp(e.CI_Empleado, ciModificar) == 0 && !e.eliminado) {
            encontrado = true;
            
            cout << endl << "EMPLEADO ENCONTRADO:" << endl;
            cout << "CI: " << e.CI_Empleado << endl;
            cout << "Nombre: " << e.nombre << " " << e.apellido << endl;
            cout << "Rango: " << e.rango << endl;
            
            do {
                cout << endl << "Que dato desea modificar?" << endl;
                cout << "1. Nombre" << endl;
                cout << "2. Apellido" << endl;
                cout << "3. Rango" << endl;
                cout << "4. Salir de modificacion" << endl;
                cout << "Seleccione una opcion: ";
                cin >> opcion;
                cin.ignore();
                
                switch(opcion) {
                    case 1:
                        cout << "Ingrese el nuevo nombre: ";
                        cin.getline(e.nombre, 30);
                        cout << "Nombre actualizado correctamente." << endl;
                        break;
                    case 2:
                        cout << "Ingrese el nuevo apellido: ";
                        cin.getline(e.apellido, 30);
                        cout << "Apellido actualizado correctamente." << endl;
                        break;
                    case 3:
                        cout << "Ingrese el nuevo rango:" << endl;
                        cout << "1. Vendedor" << endl;
                        cout << "2. Cajero" << endl;
                        cout << "Seleccione: ";
                        cin >> e.rango;
                        cin.ignore();
                        cout << "Rango actualizado correctamente." << endl;
                        break;
                    case 4:
                        cout << "Saliendo de la modificacion..." << endl;
                        break;
                    default:
                        cout << "Opcion no valida. Intente de nuevo." << endl;
                }
                
            } while (opcion != 4);
            
            archivo.seekp(-sizeof(DatosEmpleado), ios::cur);
            archivo.write((char*)&e, sizeof(DatosEmpleado));
            archivo.flush();
        }
    }
    
    archivo.close();
    
    if (!encontrado) {
        cout << "No se encontro empleado activo con CI: " << ciModificar << endl;
    } else {
        cout << "Empleado modificado correctamente." << endl;
    }
}

// ==================== FUNCIONES DE VENTAS ====================

bool buscar_producto_por_codigo(int codigo, DatosProducto &producto_encontrado) {
    ifstream archivo_productos;
    
    archivo_productos.open("productos.bin", ios::binary);
    if (!archivo_productos.is_open()) {
        cout << "No se pudo abrir el archivo de productos." << endl;
        return false;
    }
    
    while (archivo_productos.read((char*)&producto_encontrado, sizeof(DatosProducto))) {
        if (producto_encontrado.codigo == codigo && !producto_encontrado.eliminado) {
            archivo_productos.close();
            return true;
        }
    }
    
    archivo_productos.close();
    return false;
}

void registrarVenta() {
    ventaProducto venta;
    DatosProducto producto;
    ofstream archivo_ventas;

    cout << endl << "REGISTRAR NUEVA VENTA" << endl;
    
    archivo_ventas.open("ventas.bin", ios::binary | ios::app);
    if (!archivo_ventas.is_open()) {
        cout << "Error: no se pudo crear/abrir ventas.bin" << endl;
        return;
    }
   
    cin.ignore();
    cout << "Ingresar CI del cliente: ";
    cin.getline(venta.CI_Cliente, 10);

    cout << "Ingresar CI del empleado: ";
    cin.getline(venta.CI_Empleado, 10);

    cout << "Ingresar fecha de venta" << endl;
    cout << "Dia: ";
    cin >> venta.fechaDeVenta.dia;
    cout << "Mes: ";
    cin >> venta.fechaDeVenta.mes;
    cout << "Anio: ";
    cin >> venta.fechaDeVenta.anio;
    
    cout << "Ingresar codigo del producto: ";
    cin >> venta.codigoProducto;

    if (!buscar_producto_por_codigo(venta.codigoProducto, producto)) {
        cout << "Producto no encontrado o eliminado." << endl;
        archivo_ventas.close();
        return;
    }

    cout << "Producto: " << producto.modelo << endl;
    cout << "Precio de venta: " << producto.precioVenta << endl;
    cout << "Stock disponible: " << producto.stock << endl;
    venta.precioUnitario = producto.precioVenta;
    
    cout << "Ingresar cantidad a vender: ";
    cin >> venta.cantidad;

    if (venta.cantidad > producto.stock) {
        cout << "Stock insuficiente. Solo hay " << producto.stock << " unidades disponibles." << endl;
        archivo_ventas.close();
        return;
    }
    
    if (venta.cantidad <= 0) {
        cout << "Cantidad no valida." << endl;
        archivo_ventas.close();
        return;
    }
    
    venta.eliminada = false;
    venta.facturada = true; // Toda venta se factura automaticamente
    venta.numeroFactura = 0; // Simplificacion

    archivo_ventas.write((char*)&venta, sizeof(ventaProducto));
    archivo_ventas.close();

    cout << "Venta registrada correctamente." << endl;
    
    // Actualizar stock del producto
    fstream archivo_productos;
    archivo_productos.open("productos.bin", ios::binary | ios::in | ios::out);
    if (archivo_productos.is_open()) {
        DatosProducto producto_actual;
        while (archivo_productos.read((char*)&producto_actual, sizeof(DatosProducto))) {
            if (producto_actual.codigo == venta.codigoProducto && !producto_actual.eliminado) {
                archivo_productos.seekp(-sizeof(DatosProducto), ios::cur);
                producto_actual.stock -= venta.cantidad;
                archivo_productos.write((char*)&producto_actual, sizeof(DatosProducto));
                break;
            }
        }
        archivo_productos.close();
    }
}

void mostrarVentasRegistradas() {
    ifstream archivo_ventas;
    ventaProducto venta;
    int contador = 0;
    float totalGeneral = 0;

    archivo_ventas.open("ventas.bin", ios::binary);
    if (!archivo_ventas.is_open()) {
        cout << "No hay ventas registradas." << endl;
        return;
    }

    cout << endl << "=== VENTAS REGISTRADAS ===" << endl;
    cout << "No.\tCLIENTE\t\tEMPLEADO\tFECHA\t\tPRODUCTO\tCANTIDAD\tTOTAL" << endl;
    cout << "------------------------------------------------------------------------" << endl;

    while (archivo_ventas.read((char*)&venta, sizeof(ventaProducto))) {
        if (!venta.eliminada) {
            contador++;
            float totalVenta = venta.cantidad * venta.precioUnitario;
            totalGeneral += totalVenta;
            
            cout << contador << "\t"
                 << venta.CI_Cliente << "\t"
                 << venta.CI_Empleado << "\t"
                 << venta.fechaDeVenta.dia << "/" << venta.fechaDeVenta.mes << "/" << venta.fechaDeVenta.anio << "\t"
                 << venta.codigoProducto << "\t\t"
                 << venta.cantidad << "\t\t"
                 << totalVenta << endl;
        }
    }

    archivo_ventas.close();

    if (contador == 0) {
        cout << "No hay ventas registradas." << endl;
    } else {
        cout << endl << "Total de ventas: " << contador << endl;
        cout << "Total general: " << totalGeneral << endl;
    }
}

// ==================== FUNCIONES DE REPORTES SIMPLIFICADAS ====================

void reporteVentasPorCliente() {
    ifstream archivo_ventas;
    ventaProducto venta;
    char CI_a_buscar[10];
    float subtotal = 0.0;
    float total = 0.0;

    cin.ignore();
    cout << "Ingresar CI del cliente: ";
    cin.getline(CI_a_buscar, 10);

    archivo_ventas.open("ventas.bin", ios::binary);
    if (!archivo_ventas.is_open()) {
        cout << "No se pudo abrir el archivo de ventas." << endl;
        return;
    }

    cout << "=== VENTAS POR CLIENTE: " << CI_a_buscar << " ===" << endl;
    cout << "CODIGO\tFECHA\t\tCANTIDAD\tPRECIO\tSUBTOTAL" << endl;

    while (archivo_ventas.read((char*)&venta, sizeof(ventaProducto))) {
        if (strcmp(venta.CI_Cliente, CI_a_buscar) == 0 && !venta.eliminada) {
            subtotal = venta.cantidad * venta.precioUnitario;
            total = total + subtotal;

            cout << venta.codigoProducto << "\t" 
                 << venta.fechaDeVenta.dia << "/" << venta.fechaDeVenta.mes << "/" << venta.fechaDeVenta.anio << "\t"
                 << venta.cantidad << "\t\t" << venta.precioUnitario << "\t" << subtotal << endl;
        }
    }

    archivo_ventas.close();

    cout << "TOTAL COMPRADO: $" << total << endl;
}

void reporteVentasDiarias() {
    ifstream archivo_ventas;
    ventaProducto venta;
    Fecha fecha;
    float subtotal = 0.0;
    float total = 0.0;

    cout << "Ingresar fecha" << endl;
    cout << "Dia: ";
    cin >> fecha.dia;
    cout << "Mes: ";
    cin >> fecha.mes;
    cout << "Anio: ";
    cin >> fecha.anio;

    archivo_ventas.open("ventas.bin", ios::binary);
    if (!archivo_ventas.is_open()) {
        cout << "No se pudo abrir el archivo de ventas." << endl;
        return;
    }

    cout << "=== VENTAS DEL DIA: " << fecha.dia << "/" << fecha.mes << "/" << fecha.anio << " ===" << endl;
    cout << "CODIGO\tCI CLIENTE\tCANTIDAD\tSUBTOTAL" << endl;

    while (archivo_ventas.read((char*)&venta, sizeof(ventaProducto))) {
        if (!venta.eliminada && venta.fechaDeVenta.dia == fecha.dia && 
            venta.fechaDeVenta.mes == fecha.mes && venta.fechaDeVenta.anio == fecha.anio) {
            subtotal = venta.cantidad * venta.precioUnitario;
            total = total + subtotal;

            cout << venta.codigoProducto << "\t" << venta.CI_Cliente << "\t\t" 
                 << venta.cantidad << "\t\t" << subtotal << endl;
        }
    }

    archivo_ventas.close();
    cout << "TOTAL DEL DIA: $" << total << endl;
}

void reporteClientesMasCompras() {
    ifstream archivo_ventas;
    ventaProducto venta;
    
    // Primero, obtener todos los clientes diferentes que han comprado
    vector<string> clientes;
    vector<float> totales;
    
    archivo_ventas.open("ventas.bin", ios::binary);
    if (!archivo_ventas.is_open()) {
        cout << "No hay ventas registradas para generar el reporte." << endl;
        return;
    }
    
    // Leer todas las ventas
    while (archivo_ventas.read((char*)&venta, sizeof(ventaProducto))) {
        if (!venta.eliminada) {
            float subtotal = venta.cantidad * venta.precioUnitario;
            
            // Buscar si el cliente ya esta en la lista
            bool encontrado = false;
            for (size_t i = 0; i < clientes.size(); i++) {
                if (clientes[i] == venta.CI_Cliente) {
                    totales[i] += subtotal;
                    encontrado = true;
                    break;
                }
            }
            
            if (!encontrado) {
                clientes.push_back(venta.CI_Cliente);
                totales.push_back(subtotal);
            }
        }
    }
    archivo_ventas.close();
    
    if (clientes.empty()) {
        cout << "No hay clientes con compras registradas." << endl;
        return;
    }
    
    // Ordenar clientes por total comprado (ordenamiento simple burbuja)
    for (size_t i = 0; i < clientes.size() - 1; i++) {
        for (size_t j = i + 1; j < clientes.size(); j++) {
            if (totales[j] > totales[i]) {
                // Intercambiar clientes
                string tempCliente = clientes[i];
                clientes[i] = clientes[j];
                clientes[j] = tempCliente;
                
                // Intercambiar totales
                float tempTotal = totales[i];
                totales[i] = totales[j];
                totales[j] = tempTotal;
            }
        }
    }
    
    cout << endl << "=== REPORTE: CLIENTES QUE MAS COMPRAN ===" << endl;
    cout << "TOP 10 CLIENTES:" << endl;
    cout << "==========================================" << endl;
    
    int limite = (clientes.size() < 10) ? clientes.size() : 10;
    
    for (int i = 0; i < limite; i++) {
        cout << i+1 << ". CI: " << clientes[i] << " - Total comprado: $" << totales[i] << endl;
    }
    
    cout << "==========================================" << endl;
    cout << "Total de clientes con compras: " << clientes.size() << endl;
}

void reporteMesMasVentas() {
    ifstream archivo_ventas;
    ventaProducto venta;
    
    // Array para meses (1-12)
    string nombresMeses[] = {"", "Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio","Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};
    
    // Contadores para cada mes
    float ventasPorMes[13] = {0}; // Indice 0 no usado
    int cantidadPorMes[13] = {0};
    
    archivo_ventas.open("ventas.bin", ios::binary);
    if (!archivo_ventas.is_open()) {
        cout << "No hay ventas registradas para generar el reporte." << endl;
        return;
    }
    
    // Calcular ventas por mes
    while (archivo_ventas.read((char*)&venta, sizeof(ventaProducto))) {
        if (!venta.eliminada) {
            int mes = venta.fechaDeVenta.mes;
            if (mes >= 1 && mes <= 12) {
                float subtotal = venta.cantidad * venta.precioUnitario;
                ventasPorMes[mes] += subtotal;
                cantidadPorMes[mes] += venta.cantidad;
            }
        }
    }
    archivo_ventas.close();
    
    // Encontrar mes con mas ventas
    int mesTop = 0;
    float maxVentasMes = 0;
    int maxCantidad = 0;
    
    for (int mes = 1; mes <= 12; mes++) {
        if (ventasPorMes[mes] > maxVentasMes) {
            maxVentasMes = ventasPorMes[mes];
            maxCantidad = cantidadPorMes[mes];
            mesTop = mes;
        }
    }
    
    cout << endl << "=== REPORTE: MES CON MAS VENTAS ===" << endl;
    
    if (mesTop > 0) {
        cout << "Mes con mas ventas: " << nombresMeses[mesTop] << endl;
        cout << "Ventas totales del mes: $" << maxVentasMes << endl;
        cout << "Cantidad de productos vendidos: " << maxCantidad << endl;
    } else {
        cout << "No se encontraron ventas registradas." << endl;
    }
    
    // Mostrar ventas por todos los meses
    cout << endl << "VENTAS POR MES:" << endl;
    cout << "==================" << endl;
    
    float totalAnual = 0;
    int totalProductosAnual = 0;
    
    for (int mes = 1; mes <= 12; mes++) {
        if (ventasPorMes[mes] > 0) {
            cout << nombresMeses[mes] << ": $" << ventasPorMes[mes] 
                 << " (" << cantidadPorMes[mes] << " productos)" << endl;
            totalAnual += ventasPorMes[mes];
            totalProductosAnual += cantidadPorMes[mes];
        }
    }
    
    cout << endl << "Total anual: $" << totalAnual << endl;
    cout << "Total productos vendidos anual: " << totalProductosAnual << endl;
}

void reporteEmpleadoMasVende() {
    ifstream archivo_ventas;
    ventaProducto venta;
    
    // Vectores para almacenar empleados y sus ventas
    vector<string> empleados;
    vector<float> ventasEmpleados;
    
    archivo_ventas.open("ventas.bin", ios::binary);
    if (!archivo_ventas.is_open()) {
        cout << "No hay ventas registradas para generar el reporte." << endl;
        return;
    }
    
    // Procesar todas las ventas
    while (archivo_ventas.read((char*)&venta, sizeof(ventaProducto))) {
        if (!venta.eliminada) {
            float subtotal = venta.cantidad * venta.precioUnitario;
            
            // Buscar si el empleado ya esta en la lista
            bool encontrado = false;
            for (size_t i = 0; i < empleados.size(); i++) {
                if (empleados[i] == venta.CI_Empleado) {
                    ventasEmpleados[i] += subtotal;
                    encontrado = true;
                    break;
                }
            }
            
            // Si no esta, agregarlo
            if (!encontrado) {
                empleados.push_back(venta.CI_Empleado);
                ventasEmpleados.push_back(subtotal);
            }
        }
    }
    archivo_ventas.close();
    
    if (empleados.empty()) {
        cout << "No hay empleados con ventas registradas." << endl;
        return;
    }
    
    // Ordenar empleados por ventas
    for (size_t i = 0; i < empleados.size() - 1; i++) {
        for (size_t j = i + 1; j < empleados.size(); j++) {
            if (ventasEmpleados[j] > ventasEmpleados[i]) {
                // Intercambiar empleados
                string tempEmp = empleados[i];
                empleados[i] = empleados[j];
                empleados[j] = tempEmp;
                
                // Intercambiar ventas
                float tempVenta = ventasEmpleados[i];
                ventasEmpleados[i] = ventasEmpleados[j];
                ventasEmpleados[j] = tempVenta;
            }
        }
    }
    
    cout << endl << "=== REPORTE: EMPLEADO QUE MAS VENDE ===" << endl;
    
    // Mostrar el empleado top
    if (!empleados.empty()) {
        cout << "Empleado con mas ventas:" << endl;
        cout << "CI: " << empleados[0] << endl;
        cout << "Total vendido: $" << ventasEmpleados[0] << endl;
        
        // Buscar informacion del empleado en el archivo
        ifstream archivoEmpleados("empleado.bin", ios::binary);
        DatosEmpleado empleado;
        bool encontrado = false;
        
        while (archivoEmpleados.read((char*)&empleado, sizeof(DatosEmpleado))) {
            if (strcmp(empleado.CI_Empleado, empleados[0].c_str()) == 0 && !empleado.eliminado) {
                encontrado = true;
                cout << "Nombre: " << empleado.nombre << " " << empleado.apellido << endl;
                cout << "Rango: ";
                if (empleado.rango == 1) cout << "Vendedor";
                else if (empleado.rango == 2) cout << "Cajero";
                else cout << empleado.rango;
                cout << endl;
                break;
            }
        }
        archivoEmpleados.close();
        
        if (!encontrado) {
            cout << "Empleado no encontrado en el registro actual." << endl;
        }
    }
    
    // Mostrar ranking completo
    cout << endl << "RANKING COMPLETO DE EMPLEADOS:" << endl;
    cout << "==================================" << endl;
    
    for (size_t i = 0; i < empleados.size(); i++) {
        cout << i+1 << ". CI: " << empleados[i] << " - Ventas: $" << ventasEmpleados[i] << endl;
    }
}

// ==================== MENU ACTUALIZADO ====================

void Menu() {
    DatosCliente c;
    DatosEmpleado e;
    int opcion;
    
    do {
        cout << endl << "=== MENU PRINCIPAL ===" << endl;
        cout << "1. Llenar datos del cliente (se guarda automaticamente)" << endl;
        cout << "2. Mostrar todos los clientes registrados (solo activos)" << endl;
        cout << "3. Eliminar cliente por CI (eliminacion logica)" << endl;
        cout << "4. Modificar cliente por CI" << endl;
        cout << "5. Llenar datos del empleado (se guarda automaticamente)" << endl;
        cout << "6. Mostrar todos los empleados registrados (solo activos)" << endl;
        cout << "7. Eliminar empleado por CI (eliminacion logica)" << endl;
        cout << "8. Modificar empleado por CI" << endl;
        cout << "9. Gestion de productos" << endl;
        cout << "10. Registrar nueva venta" << endl;
        cout << "11. Mostrar ventas registradas" << endl;
        cout << "12. Reporte: Ventas por cliente" << endl;
        cout << "13. Reporte: Ventas diarias" << endl;
        cout << "14. Reporte: Clientes que mas compran" << endl;
        cout << "15. Reporte: Empleado que mas vende" << endl;
        cout << "16. Reporte: Mes con mas ventas" << endl;
        cout << "17. Salir" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
        case 1:
            llenarDatosCliente(c);
            break;
        case 2:
            mostrarTodosClientes();
            break;
        case 3:
            eliminarCliente();
            break;
        case 4:
            modificarCliente();
            break;
        case 5:
            llenarDatosEmpleado(e);
            break;
        case 6:
            mostrarTodosEmpleados();
            break;
        case 7:
            eliminarEmpleado();
            break;
        case 8:
            modificarEmpleado();
            break;
        case 9:
            menuProductos();
            break;
        case 10:
            registrarVenta();
            break;
        case 11:
            mostrarVentasRegistradas();
            break;
        case 12:
            reporteVentasPorCliente();
            break;
        case 13:
            reporteVentasDiarias();
            break;
        case 14:
            reporteClientesMasCompras();
            break;
        case 15:
            reporteEmpleadoMasVende();
            break;
        case 16:
            reporteMesMasVentas();
            break;
        case 17:
            cout << "Saliendo del programa..." << endl;
            break;
        default:
            cout << "Opcion no valida. Intente de nuevo." << endl;
        }
    } while (opcion != 17);
}

int main() {
    Menu();
    return 0;
}