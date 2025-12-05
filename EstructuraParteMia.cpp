#include <iostream>
#include <fstream>
using namespace std;
// ESTRUCTURA FECHA
struct Fecha{
    int dia;
    int mes;
    int anio;
};
// ESTRUCTURA CLIENTE
struct Cliente{
    char CIcliente[10]; 
    char nombreCliente[30];
    char apellidoCliente[30];
    Fecha fechaNacimiento;
    int telefono;
    char correo[30];
};
//ESTRUCTURA EMPLEADO
struct Empleado{
    char CIempleado[10];
    char nombreEmpleado[30];
    char apellidoEmpleado[30];
    char rangoEmpleado[30];
};

void llenarDatosCliente(Cliente &c){
    cout<<endl<<"INGRESO DE DATOS DEL CLIENTE"<<endl;
    cout<<"Ingrese el CI del cliente: ";
    cin.getline(c.CIcliente,10);
    cout<<"Ingrese el nombre del cliente: ";
    cin.getline(c.nombreCliente,30);
    cout<<"Ingrese el apellido del cliente: ";
    cin.getline(c.apellidoCliente,30);
    cout<<"Ingrese la fecha de nacimiento del cliente"<<endl;
    cout<<"Ingrese el Dia: ";
    cin>>c.fechaNacimiento.dia;
    cout<<"Ingrese el Mes: ";
    cin>>c.fechaNacimiento.mes;
    cout<<"Ingrese el Anio: ";
    cin>>c.fechaNacimiento.anio;
    cout<<"Ingrese el telefono del cliente: ";
    cin>>c.telefono;
    cin.ignore();
    cout<<"Ingrese el correo del cliente: ";
    cin.getline(c.correo,30);
    cout<<endl;
}

void mostrarDatosCliente(Cliente c){
    cout<<endl<<"DATOS DEL CLIENTE"<<endl;
    cout<<"El CI del cliente es: "<<c.CIcliente<<endl;
    cout<<"El nombre del cliente es: "<<c.nombreCliente<<endl;
    cout<<"El apellido del cliente es: "<<c.apellidoCliente<<endl;
    cout<<"La fecha de nacimiento del cliente es: "<<c.fechaNacimiento.dia<<"/"<<c.fechaNacimiento.mes<<"/"<<c.fechaNacimiento.anio<<endl;
    cout<<"El telefono del cliente es: "<<c.telefono<<endl;
    cout<<"El correo del cliente es: "<<c.correo<<endl;

    ofstream archivo;
    archivo.open("clientes.bin", ios::binary | ios::app);

    if (!archivo.is_open())
    {
        cout<<"Error no se pudo guardar clientes.bin";
        return;
    }
    archivo.write((char*)&c, sizeof(Cliente));
    archivo.close();
    cout<<"Cliente guardado"<<endl;
}

void llenarDatosEmpleado(Empleado &e){
    cout<<endl<<"INGRESO DE DATOS EMPLEADO"<<endl;
    cout<<"Ingrese el CI del empleado: ";
    cin.getline(e.CIempleado,10);
    cout<<"Ingrese el nombre del empleado: ";
    cin.getline(e.nombreEmpleado,30);
    cout<<"Ingrese el apellido del empleado: ";
    cin.getline(e.apellidoEmpleado,30);
    cout<<"Ingrese el rango del empleado: ";
    cin.getline(e.rangoEmpleado,30);
}

void mostrarDatosEmpleado(Empleado e){
    cout<<endl<<"DATOS EMPLEADO"<<endl;
    cout<<"El CI del empleado es: "<<e.CIempleado<<endl;
    cout<<"El nombre del empleado es: "<<e.nombreEmpleado<<endl;
    cout<<"El apellido del empleado es: "<<e.apellidoEmpleado<<endl;
    cout<<"El rango del empleado es: "<<e.rangoEmpleado<<endl;

    ofstream archivo;
    archivo.open("empleado.bin", ios::binary | ios::app);

    if (!archivo)
    {
        cout<<"Error no se pudo crear empleado.bin"<<endl;
    }

    archivo.write((char*)&e, sizeof(Empleado));
    archivo.close();
    cout<<"Empleado guardado en empleados.bin"<<endl;
}

void Menu(){
    Cliente c;
    Empleado e;
    int opcion;
    
    do
    {
        cout<<endl<<"MENU"<<endl;
        cout<<"1. Llenar datos del cliente"<<endl;
        cout<<"2. Mostrar datos del cliente"<<endl;
        cout<<"3. Llenar datos del emplado"<<endl;
        cout<<"4. Mostrar datos del empleado"<<endl;
        cin>>opcion;
        cin.ignore();

        switch (opcion)
        {
        case 1:
            llenarDatosCliente(c);
            break;
        case 2:
            mostrarDatosCliente(c);
            break;
        case 3:
            llenarDatosEmpleado(e);
            break;
        case 4:
            llenarDatosEmpleado(e);
            break;
        default:
            break;
        }
    } while (opcion != 4);
}

int main()
{
    
    Menu();
    
    
    return 0;
}