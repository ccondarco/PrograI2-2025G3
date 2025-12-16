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
    char CI_Cliente[30];
    char nombre[30];
    char apellido[30];
    Fecha fechaNacimiento;
    int telefono;
    char correo[30];
};

struct DatosEmpleado {
    char CI_Empleado[10];
    char nombre[30];
    char apellido[30];
    int rango;
};

void llenarDatosCliente(DatosCliente &c) {
    cout << endl << "INGRESO DE DATOS DEL CLIENTE" << endl;
    cout << "Ingrese el CI del cliente: ";
    cin.getline(c.CI_Cliente, 30);
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
    cout << endl;
}

void mostrarDatosCliente(DatosCliente c) {
    cout << endl << "DATOS DEL CLIENTE" << endl;
    cout << "El CI del cliente es: " << c.CI_Cliente << endl;
    cout << "El nombre del cliente es: " << c.nombre << endl;
    cout << "El apellido del cliente es: " << c.apellido << endl;
    cout << "La fecha de nacimiento del cliente es: " << c.fechaNacimiento.mes << "/" << c.fechaNacimiento.dia << "/" << c.fechaNacimiento.anio << endl;
    cout << "El telefono del cliente es: " << c.telefono << endl;
    cout << "El correo del cliente es: " << c.correo << endl;

    ofstream archivo;
    archivo.open("clientes.bin", ios::binary | ios::app);

    if (!archivo.is_open()) {
        cout << "Error no se pudo guardar clientes.bin";
        return;
    }
    archivo.write((char*)&c, sizeof(DatosCliente));
    archivo.close();
    cout << "Cliente guardado en clientes.bin" << endl;
}

void llenarDatosEmpleado(DatosEmpleado &e) {
    cout << endl << "INGRESO DE DATOS EMPLEADO" << endl;
    cout << "Ingrese el CI del empleado: ";
    cin.getline(e.CI_Empleado, 10);
    cout << "Ingrese el nombre del empleado: ";
    cin.getline(e.nombre, 30);
    cout << "Ingrese el apellido del empleado: ";
    cin.getline(e.apellido, 30);
    cout << "Ingrese el rango del empleado (numero entero): ";
    cin >> e.rango;
    cin.ignore();
}

void mostrarDatosEmpleado(DatosEmpleado e) {
    cout << endl << "DATOS EMPLEADO" << endl;
    cout << "El CI del empleado es: " << e.CI_Empleado << endl;
    cout << "El nombre del empleado es: " << e.nombre << endl;
    cout << "El apellido del empleado es: " << e.apellido << endl;
    cout << "El rango del empleado es: " << e.rango << endl;

    ofstream archivo;
    archivo.open("empleado.bin", ios::binary | ios::app);

    if (!archivo) {
        cout << "Error no se pudo crear empleado.bin" << endl;
        return;
    }

    archivo.write((char*)&e, sizeof(DatosEmpleado));
    archivo.close();
    cout << "Empleado guardado en empleado.bin" << endl;
}

void mostrarTodosClientes() {
    ifstream archivo("clientes.bin", ios::binary);
    
    if (!archivo.is_open()) {
        cout << "No hay clientes registrados o error al abrir el archivo." << endl;
        return;
    }
    
    DatosCliente c;
    int contador = 1;
    
    cout << endl << "LISTA DE TODOS LOS CLIENTES" << endl;
    
    while (archivo.read((char*)&c, sizeof(DatosCliente))) {
        cout << endl << "CLIENTE " << contador << ":" << endl;
        cout << "CI: " << c.CI_Cliente << endl;
        cout << "Nombre: " << c.nombre << " " << c.apellido << endl;
        cout << "Telefono: " << c.telefono << endl;
        cout << "Correo: " << c.correo << endl;
        contador++;
    }
    
    archivo.close();
}

void eliminarCliente() {
    char ciEliminar[30];
    cout << endl << "ELIMINAR CLIENTE" << endl;
    cout << "Ingrese el CI del cliente a eliminar: ";
    cin.getline(ciEliminar, 30);
    
    ifstream archivoEntrada("clientes.bin", ios::binary);
    ofstream archivoTemp("temp.bin", ios::binary);
    
    if (!archivoEntrada.is_open() || !archivoTemp.is_open()) {
        cout << "Error al abrir los archivos." << endl;
        return;
    }
    
    DatosCliente c;
    bool encontrado = false;
    int eliminados = 0;
    
    while (archivoEntrada.read((char*)&c, sizeof(DatosCliente))) {
        if (strcmp(c.CI_Cliente, ciEliminar) == 0) {
            cout << "Cliente encontrado y eliminado: " << c.nombre << " " << c.apellido << endl;
            encontrado = true;
            eliminados++;
        } else {
            archivoTemp.write((char*)&c, sizeof(DatosCliente));
        }
    }
    
    archivoEntrada.close();
    archivoTemp.close();
    
    if (encontrado) {
        remove("clientes.bin");
        rename("temp.bin", "clientes.bin");
        cout << eliminados << " cliente eliminado" << endl;
    } else {
        cout << "No se encontro cliente con CI: " << ciEliminar << endl;
        remove("temp.bin");  // Eliminar archivo temporal si no se hizo nada
    }
}

void mostrarTodosEmpleados() {
    ifstream archivo("empleado.bin", ios::binary);
    
    if (!archivo.is_open()) {
        cout << "No hay empleados registrados o error al abrir el archivo." << endl;
        return;
    }
    
    DatosEmpleado e;
    int contador = 1;
    
    cout << endl << "LISTA DE TODOS LOS EMPLEADOS"<<endl;
    
    while (archivo.read((char*)&e, sizeof(DatosEmpleado))) {
        cout << endl << "EMPLEADO " << contador << ":" << endl;
        cout << "CI: " << e.CI_Empleado << endl;
        cout << "Nombre: " << e.nombre << " " << e.apellido << endl;
        cout << "Rango: " << e.rango << endl;
        contador++;
    }
    
    archivo.close();
}

// ELIMINAR UN EMPLEADO POR CI
void eliminarEmpleado() {
    char ciEliminar[10];
    cout << endl << "=== ELIMINAR EMPLEADO ===" << endl;
    cout << "Ingrese el CI del empleado a eliminar: ";
    cin.getline(ciEliminar, 10);
    
    ifstream archivoEntrada("empleado.bin", ios::binary);
    ofstream archivoTemp("temp_emp.bin", ios::binary);
    
    if (!archivoEntrada.is_open() || !archivoTemp.is_open()) {
        cout << "Error al abrir los archivos." << endl;
        return;
    }
    
    DatosEmpleado e;
    bool encontrado = false;
    int eliminados = 0;
    
    while (archivoEntrada.read((char*)&e, sizeof(DatosEmpleado))) {
        if (strcmp(e.CI_Empleado, ciEliminar) == 0) {
            cout << "Empleado encontrado y eliminado: " << e.nombre << " " << e.apellido << endl;
            encontrado = true;
            eliminados++;
        } else {
            archivoTemp.write((char*)&e, sizeof(DatosEmpleado));
        }
    }
    
    archivoEntrada.close();
    archivoTemp.close();
    
    if (encontrado) {
        remove("empleado.bin");
        rename("temp_emp.bin", "empleado.bin");
        cout << eliminados << " empleado(s) eliminado(s) correctamente." << endl;
    } else {
        cout << "No se encontro empleado con CI: " << ciEliminar << endl;
        remove("temp_emp.bin");  // Eliminar archivo temporal si no se hizo nada
    }
}

void Menu() {
    DatosCliente c;
    DatosEmpleado e;
    int opcion;
    
    do {
        cout << endl << "=== MENU PRINCIPAL ===" << endl;
        cout << "1. Llenar datos del cliente" << endl;
        cout << "2. Mostrar datos del cliente actual (y guardar)" << endl;
        cout << "3. Mostrar todos los clientes registrados" << endl;
        cout << "4. Eliminar cliente por CI" << endl;
        cout << "5. Llenar datos del empleado" << endl;
        cout << "6. Mostrar datos del empleado (y guardar)" << endl;
        cout << "7. Mostrar todos los empleados registrados" << endl;
        cout << "8. Eliminar empleado por CI" << endl;
        cout << "9. Salir" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
        case 1:
            llenarDatosCliente(c);
            break;
        case 2:
            mostrarDatosCliente(c);
            break;
        case 3:
            mostrarTodosClientes();
            break;
        case 4:
            eliminarCliente();
            break;
        case 5:
            llenarDatosEmpleado(e);
            break;
        case 6:
            mostrarDatosEmpleado(e);
            break;
        case 7:
            mostrarTodosEmpleados();
            break;
        case 8:
            eliminarEmpleado();
            break;
        case 9:
            cout << "Saliendo del programa..." << endl;
            break;
        default:
            cout << "Opcion no valida. Intente de nuevo." << endl;
            break;
        }
    } while (opcion != 9);
}

int main() {
    Menu();
    return 0;
}