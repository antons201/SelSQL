
#include "Logic/Headers/MainLogic.h"
#include "Parser/Headers/TreeVisitor.h"
#include "Utils/Structures/BigResponse.h"
#include "parser.cpp"

[[noreturn]] int main() {
    std::string str;
    BigResponse result;
    auto visitor = new TreeVisitor();
    while (true) {
        std::getline(std::cin, str);
        auto tree = parse_request(str.c_str());
        if (tree == nullptr) {
            std::cerr << "PARSE ERROR" << std::endl;
        } else {
            tree->accept(visitor);
            auto error = visitor->getError();
            if (error.getErrorCode())
                std::cerr << error.getErrorMsg() << " ERROR: " << error.getErrorCode() << std::endl;
//            else if (result->ddlData.returnMsg.size() > 0) {
//                std::cout << result->ddlData.returnMsg << std::endl;
//            }
        }
        printf("ENTER NEW COMMAND\n");
    }
    delete visitor;
}