#include <iostream>
#include <fstream>
#include <cstring>  
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
    
    // Inicializar como no eliminado
    c.eliminado = false;
    
    cout << endl;
    
    // GUARDAR EL CLIENTE INMEDIATAMENTE DESPUES DE LLENAR LOS DATOS
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

// Funcion: mostrarDatosCliente
// Descripcion: Muestra en pantalla los datos de un cliente almacenado en memoria
// Parametro: c - estructura DatosCliente con los datos a mostrar
// Retorno: void
void mostrarDatosCliente(DatosCliente c) {
    // SOLO MUESTRA, NO GUARDA - para evitar duplicados
    cout << endl << "DATOS DEL CLIENTE ACTUAL" << endl;
    cout << "El CI del cliente es: " << c.CI_Cliente << endl;
    cout << "El nombre del cliente es: " << c.nombre << endl;
    cout << "El apellido del cliente es: " << c.apellido << endl;
    cout << "La fecha de nacimiento del cliente es: " << c.fechaNacimiento.mes << "/" << c.fechaNacimiento.dia << "/" << c.fechaNacimiento.anio << endl;
    cout << "El telefono del cliente es: " << c.telefono << endl;
    cout << "El correo del cliente es: " << c.correo << endl;
    cout << "Estado: " << (c.eliminado ? "ELIMINADO" : "ACTIVO") << endl;
}

// Funcion: llenarDatosEmpleado
// Descripcion: Solicita al usuario los datos de un empleado y los guarda en el archivo binario
// Parametro: e - referencia a la estructura DatosEmpleado donde se almacenan los datos
// Retorno: void
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
    
    // Inicializar como no eliminado
    e.eliminado = false;
    
    // GUARDAR EL EMPLEADO INMEDIATAMENTE DESPUES DE LLENAR LOS DATOS
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
    // SOLO MUESTRA, NO GUARDA - para evitar duplicados
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
    
    cout << endl << "=== LISTA DE TODOS LOS CLIENTES (SOLO ACTIVOS) ===" << endl;
    
    // Leer todos los clientes del archivo
    while (archivo.read((char*)&c, sizeof(DatosCliente))) {
        contador++;
        if (!c.eliminado) {
            activos++;
            cout << endl << "CLIENTE " << activos << ":" << endl;
            cout << "CI: " << c.CI_Cliente << endl;
            cout << "Nombre: " << c.nombre << " " << c.apellido << endl;
            cout << "Fecha Nacimiento: " << c.fechaNacimiento.mes << "/" 
                 << c.fechaNacimiento.dia << "/" << c.fechaNacimiento.anio << endl;
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
    
    // Buscar el cliente
    while (archivo.read((char*)&c, sizeof(DatosCliente)) && !encontrado) {
        if (strcmp(c.CI_Cliente, ciEliminar) == 0 && !c.eliminado) {
            encontrado = true;
            cout << "Cliente encontrado: " << c.nombre << " " << c.apellido << endl;
            cout << "Esta seguro de marcar como eliminado? (s/n): ";
            char confirmacion;
            cin >> confirmacion;
            cin.ignore();
            
            if (confirmacion == 's' || confirmacion == 'S') {
                // Marcar como eliminado
                c.eliminado = true;
                
                // Retroceder para sobreescribir el registro
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

// Permite modificar los datos de un cliente usando su CI
void modificarCliente() 
{
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
    
    // Buscar el cliente
    while (archivo.read((char*)&c, sizeof(DatosCliente)) && !encontrado) 
    {
        if (strcmp(c.CI_Cliente, ciModificar) == 0 && !c.eliminado) {
            encontrado = true;
            
            cout << endl << "CLIENTE ENCONTRADO:" << endl;
            cout << "CI: " << c.CI_Cliente << endl;
            cout << "Nombre: " << c.nombre << " " << c.apellido << endl;
            cout << "Telefono: " << c.telefono << endl;
            cout << "Correo: " << c.correo << endl;
            
            // Menu de modificacion
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
            
            // Retroceder para sobreescribir el registro modificado
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
    
    cout << endl << "=== LISTA DE TODOS LOS EMPLEADOS (SOLO ACTIVOS) ===" << endl;
    
    // Leer todos los empleados del archivo
    while (archivo.read((char*)&e, sizeof(DatosEmpleado))) {
        contador++;
        if (!e.eliminado) {
            activos++;
            cout << endl << "EMPLEADO " << activos << ":" << endl;
            cout << "CI: " << e.CI_Empleado << endl;
            cout << "Nombre: " << e.nombre << " " << e.apellido << endl;
            cout << "Rango: " << e.rango << " (";
            if (e.rango == 1) cout << "Vendedor";
            else if (e.rango == 2) cout << "Cajero";
            else cout << "Desconocido";
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
    
    cout << endl << "ELIMINAR EMPLEADO (ELIMINACION LOGICA)" << endl;
    cout << "Ingrese el CI del empleado a eliminar: ";
    cin.getline(ciEliminar, 10);
    
    archivo.open("empleado.bin", ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) {
        cout << "No hay empleados registrados para eliminar." << endl;
        return;
    }
    
    // Buscar el empleado
    while (archivo.read((char*)&e, sizeof(DatosEmpleado)) && !encontrado) {
        if (strcmp(e.CI_Empleado, ciEliminar) == 0 && !e.eliminado) {
            encontrado = true;
            cout << "Empleado encontrado: " << e.nombre << " " << e.apellido << endl;
            cout << "Esta seguro de marcar como eliminado? (s/n): ";
            char confirmacion;
            cin >> confirmacion;
            cin.ignore();
            
            if (confirmacion == 's' || confirmacion == 'S') {
                // Marcar como eliminado
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
    
    // Buscar el empleado
    while (archivo.read((char*)&e, sizeof(DatosEmpleado)) && !encontrado) {
        if (strcmp(e.CI_Empleado, ciModificar) == 0 && !e.eliminado) {
            encontrado = true;
            
            cout << endl << "EMPLEADO ENCONTRADO:" << endl;
            cout << "CI: " << e.CI_Empleado << endl;
            cout << "Nombre: " << e.nombre << " " << e.apellido << endl;
            cout << "Rango: " << e.rango << endl;
            
            // Menu de modificacion
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
        cout << "9. Salir" << endl;
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
            cout << "Saliendo del programa..." << endl;
            break;
        default:
            cout << "Opcion no valida. Intente de nuevo." << endl;
        }
    } while (opcion != 9);
}

int main() {
    Menu();
    return 0;
}