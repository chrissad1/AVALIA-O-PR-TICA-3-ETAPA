#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO "contas.dat"
#define SENHA_ADMIN "6424"

struct Conta {
    int numero;
    char titular[50];
    float saldo;
};

void criarConta();
void extrato();
void deposito();
void saque();
void transferencia();
void listarContas();
int buscarConta(int numero, struct Conta *conta);
void salvarConta(struct Conta conta, int posicao);
int contarContas();

int main() {
    int opcao;

    do {
        printf("\n=== SISTEMA BANCARIO ===\n");
        printf("1 - Criar conta\n");
        printf("2 - Extrato\n");
        printf("3 - Deposito\n");
        printf("4 - Saque\n");
        printf("5 - Transferencia\n");
        printf("6 - Listar todas as contas\n");
        printf("7 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar();

        switch(opcao) {
            case 1: criarConta(); break;
            case 2: extrato(); break;
            case 3: deposito(); break;
            case 4: saque(); break;
            case 5: transferencia(); break;
            case 6: listarContas(); break;
            case 7: printf("Encerrando...\n"); break;
            default: printf("Opcao invalida!\n");
        }
    } while(opcao != 7);

    return 0;
}

void criarConta() {
    struct Conta nova;
    FILE *f = fopen(ARQUIVO, "ab");

    if (!f) {
        printf("Erro ao abrir arquivo!\n");
        return;
    }

    printf("Nome do titular: ");
    fgets(nova.titular, sizeof(nova.titular), stdin);
    nova.titular[strcspn(nova.titular, "\n")] = 0;

    nova.numero = contarContas() + 1;
    nova.saldo = 0;

    fwrite(&nova, sizeof(struct Conta), 1, f);
    fclose(f);

    printf("Conta criada com sucesso! Numero: %d\n", nova.numero);
}

void extrato() {
    int numero;
    struct Conta c;

    printf("Numero da conta: ");
    scanf("%d", &numero);

    if (buscarConta(numero, &c)) {
        printf("\n=== EXTRATO ===\n");
        printf("Titular: %s\n", c.titular);
        printf("Conta: %d\n", c.numero);
        printf("Saldo: %.2f\n", c.saldo);
    } else {
        printf("Conta nao encontrada!\n");
    }
}

void deposito() {
    int numero;
    float valor;
    struct Conta c;

    printf("Numero da conta: ");
    scanf("%d", &numero);

    if (!buscarConta(numero, &c)) {
        printf("Conta nao encontrada!\n");
        return;
    }

    printf("Valor do deposito: ");
    scanf("%f", &valor);

    c.saldo += valor;
    salvarConta(c, c.numero - 1);

    printf("Deposito realizado!\n");
}

void saque() {
    int numero;
    float valor;
    struct Conta c;

    printf("Numero da conta: ");
    scanf("%d", &numero);

    if (!buscarConta(numero, &c)) {
        printf("Conta nao encontrada!\n");
        return;
    }

    printf("Valor do saque: ");
    scanf("%f", &valor);

    if (valor > c.saldo) {
        printf("Saldo insuficiente!\n");
        return;
    }

    c.saldo -= valor;
    salvarConta(c, c.numero - 1);

    printf("Saque realizado!\n");
}

void transferencia() {
    int origem, destino;
    float valor;
    struct Conta c1, c2;

    printf("Conta de origem: ");
    scanf("%d", &origem);
    printf("Conta de destino: ");
    scanf("%d", &destino);

    if (!buscarConta(origem, &c1)) {
        printf("Conta de origem nao encontrada!\n");
        return;
    }
    if (!buscarConta(destino, &c2)) {
        printf("Conta de destino nao encontrada!\n");
        return;
    }

    printf("Valor da transferencia: ");
    scanf("%f", &valor);

    if (valor > c1.saldo) {
        printf("Saldo insuficiente!\n");
        return;
    }

    c1.saldo -= valor;
    c2.saldo += valor;

    salvarConta(c1, c1.numero - 1);
    salvarConta(c2, c2.numero - 1);

    printf("Transferencia realizada!\n");
}

void listarContas() {
    char senha[20];
    FILE *f = fopen(ARQUIVO, "rb");
    struct Conta c;

    if (!f) {
        printf("Nenhuma conta cadastrada.\n");
        return;
    }

    printf("Senha de administrador: ");
    scanf("%s", senha);

    if (strcmp(senha, SENHA_ADMIN) != 0) {
        printf("Senha incorreta!\n");
        fclose(f);
        return;
    }

    printf("\n=== TODAS AS CONTAS ===\n");
    while (fread(&c, sizeof(struct Conta), 1, f)) {
        printf("Conta %d | Titular: %s | Saldo: %.2f\n", c.numero, c.titular, c.saldo);
    }

    fclose(f);
}

int buscarConta(int numero, struct Conta *conta) {
    FILE *f = fopen(ARQUIVO, "rb");
    struct Conta c;

    if (!f) return 0;

    while (fread(&c, sizeof(struct Conta), 1, f)) {
        if (c.numero == numero) {
            *conta = c;
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

void salvarConta(struct Conta conta, int posicao) {
    FILE *f = fopen(ARQUIVO, "rb+");

    if (!f) return;

    fseek(f, posicao * sizeof(struct Conta), SEEK_SET);
    fwrite(&conta, sizeof(struct Conta), 1, f);

    fclose(f);
}

int contarContas() {
    FILE *f = fopen(ARQUIVO, "rb");
    int total = 0;
    struct Conta c;

    if (!f) return 0;

    while (fread(&c, sizeof(struct Conta), 1, f)) {
        total++;
    }

    fclose(f);
    return total;
}
