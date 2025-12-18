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

struct ventaProducto {
    char CI_Cliente[10];
    char CI_Empleado[10];
    Fecha fechaDeVenta;
    int codigoProducto;
    int cantidad;
    float precioUnitario;
    bool eliminada;
};

// ==================== FUNCIONES DE GESTION DE CLIENTES ====================

void llenarDatosCliente() {
    DatosCliente c;
    cout << endl << "=== INGRESO DE NUEVO CLIENTE ===" << endl;
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
    
    cout << endl << "=== ELIMINAR CLIENTE (ELIMINACION LOGICA) ===" << endl;
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
    
    cout << endl << "=== MODIFICAR CLIENTE ===" << endl;
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

void buscarClientePorCI() {
    char ciBuscar[10];
    ifstream archivo;
    DatosCliente c;
    bool encontrado = false;
    
    cout << endl << "=== BUSCAR CLIENTE POR CI ===" << endl;
    cout << "Ingrese el CI del cliente a buscar: ";
    cin.getline(ciBuscar, 10);
    
    archivo.open("clientes.bin", ios::binary);
    if (!archivo.is_open()) {
        cout << "No hay clientes registrados." << endl;
        return;
    }
    
    while (archivo.read((char*)&c, sizeof(DatosCliente))) {
        if (strcmp(c.CI_Cliente, ciBuscar) == 0 && !c.eliminado) {
            encontrado = true;
            cout << endl << "CLIENTE ENCONTRADO:" << endl;
            cout << "CI: " << c.CI_Cliente << endl;
            cout << "Nombre: " << c.nombre << " " << c.apellido << endl;
            cout << "Fecha Nacimiento: " << c.fechaNacimiento.mes << "/" << c.fechaNacimiento.dia << "/" << c.fechaNacimiento.anio << endl;
            cout << "Telefono: " << c.telefono << endl;
            cout << "Correo: " << c.correo << endl;
            break;
        }
    }
    
    archivo.close();
    
    if (!encontrado) {
        cout << "No se encontro cliente activo con CI: " << ciBuscar << endl;
    }
}

void menuClientes() {
    int opcion;
    do {
        cout << endl << "=== MENU DE GESTION DE CLIENTES ===" << endl;
        cout << "1. Registrar nuevo cliente" << endl;
        cout << "2. Mostrar todos los clientes (solo activos)" << endl;
        cout << "3. Buscar cliente por CI" << endl;
        cout << "4. Modificar cliente por CI" << endl;
        cout << "5. Eliminar cliente por CI (eliminacion logica)" << endl;
        cout << "6. Reporte: Clientes que mas compran" << endl;
        cout << "0. Volver al menu principal" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore();
        
        switch(opcion) {
            case 1:
                llenarDatosCliente();
                break;
            case 2:
                mostrarTodosClientes();
                break;
            case 3:
                buscarClientePorCI();
                break;
            case 4:
                modificarCliente();
                break;
            case 5:
                eliminarCliente();
                break;
            case 6:
                cout << "Reporte de clientes que mas compran (requiere ventas.bin)" << endl;
                // Se implementara en la siguiente version
                break;
            case 0:
                cout << "Volviendo al menu principal..." << endl;
                break;
            default:
                cout << "Opcion no valida. Intente de nuevo." << endl;
        }
    } while (opcion != 0);
}

// ==================== FUNCIONES DE GESTION DE EMPLEADOS ====================

void llenarDatosEmpleado() {
    DatosEmpleado e;
    cout << endl << "=== INGRESO DE NUEVO EMPLEADO ===" << endl;
    cout << "Ingrese el CI del empleado: ";
    cin.getline(e.CI_Empleado, 10);
    cout << "Ingrese el nombre del empleado: ";
    cin.getline(e.nombre, 30);
    cout << "Ingrese el apellido del empleado: ";
    cin.getline(e.apellido, 30);
    cout << "Ingrese el rango del empleado:" << endl;
    cout << "1. Vendedor" << endl;
    cout << "2. Cajero" << endl;
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
    
    cout << endl << "=== ELIMINAR EMPLEADO ===" << endl;
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
    
    cout << endl << "=== MODIFICAR EMPLEADO ===" << endl;
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

void buscarEmpleadoPorCI() {
    char ciBuscar[10];
    ifstream archivo;
    DatosEmpleado e;
    bool encontrado = false;
    
    cout << endl << "=== BUSCAR EMPLEADO POR CI ===" << endl;
    cout << "Ingrese el CI del empleado a buscar: ";
    cin.getline(ciBuscar, 10);
    
    archivo.open("empleado.bin", ios::binary);
    if (!archivo.is_open()) {
        cout << "No hay empleados registrados." << endl;
        return;
    }
    
    while (archivo.read((char*)&e, sizeof(DatosEmpleado))) {
        if (strcmp(e.CI_Empleado, ciBuscar) == 0 && !e.eliminado) {
            encontrado = true;
            cout << endl << "EMPLEADO ENCONTRADO:" << endl;
            cout << "CI: " << e.CI_Empleado << endl;
            cout << "Nombre: " << e.nombre << " " << e.apellido << endl;
            cout << "Rango: " << e.rango << " (";
            if (e.rango == 1) cout << "Vendedor";
            else if (e.rango == 2) cout << "Cajero";
            else cout << "Desconocido";
            cout << ")" << endl;
            break;
        }
    }
    
    archivo.close();
    
    if (!encontrado) {
        cout << "No se encontro empleado activo con CI: " << ciBuscar << endl;
    }
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
    
    // Ordenar empleados por ventas (ordenamiento simple burbuja)
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

void menuEmpleados() {
    int opcion;
    do {
        cout << endl << "=== MENU DE GESTION DE EMPLEADOS ===" << endl;
        cout << "1. Registrar nuevo empleado" << endl;
        cout << "2. Mostrar todos los empleados (solo activos)" << endl;
        cout << "3. Buscar empleado por CI" << endl;
        cout << "4. Modificar empleado por CI" << endl;
        cout << "5. Eliminar empleado por CI (eliminacion logica)" << endl;
        cout << "6. Reporte: Empleado que mas vende" << endl;
        cout << "0. Volver al menu principal" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore();
        
        switch(opcion) {
            case 1:
                llenarDatosEmpleado();
                break;
            case 2:
                mostrarTodosEmpleados();
                break;
            case 3:
                buscarEmpleadoPorCI();
                break;
            case 4:
                modificarEmpleado();
                break;
            case 5:
                eliminarEmpleado();
                break;
            case 6:
                reporteEmpleadoMasVende();
                break;
            case 0:
                cout << "Volviendo al menu principal..." << endl;
                break;
            default:
                cout << "Opcion no valida. Intente de nuevo." << endl;
        }
    } while (opcion != 0);
}

// ==================== MENU PRINCIPAL ====================

void Menu() {
    int opcion;
    
    do {
        cout << endl << "=== MENU PRINCIPAL ===" << endl;
        cout << "1. Gestion de Clientes" << endl;
        cout << "2. Gestion de Empleados" << endl;
        cout << "3. Salir" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
        case 1:
            menuClientes();
            break;
        case 2:
            menuEmpleados();
            break;
        case 3:
            cout << "Saliendo del programa..." << endl;
            break;
        default:
            cout << "Opcion no valida. Intente de nuevo." << endl;
            break;
        }
    } while (opcion != 3);
}

int main() {
    Menu();
    return 0;
}