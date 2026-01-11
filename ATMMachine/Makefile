CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra

SOURCES = main.cpp Account.cpp Card.cpp CashDispenser.cpp TransactionLog.cpp \
          ATMMachine.cpp IdleState.cpp HasCardState.cpp SelectOperationState.cpp \
          CashWithdrawalState.cpp CheckBalanceState.cpp

HEADERS = Account.h Card.h CashDispenser.h TransactionLog.h ATMState.h \
          ATMMachine.h IdleState.h HasCardState.h SelectOperationState.h \
          CashWithdrawalState.h CheckBalanceState.h

TARGET = atm

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

clean:
	del /Q $(TARGET).exe

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run

