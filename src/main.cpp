
#include "Logic/Headers/MainLogic.h"
#include "Utils/Structures/BigResponse.h"
#include "parser.cpp"

int main() {
    std::string str;
    BigResponse result;
    while (true) {
        std::getline(std::cin, str);
        result = parse_request(str.c_str());
        if (result.error.errorCode)
            std::cerr << result.error.errorMsg << std::endl;
        else {
            result = MainLogic::executeRequest(result);
            if (result.error.errorCode)
                std::cerr << result.error.errorMsg << std::endl;
            else if (result.ddlData.returnMsg.size() > 0) {
                std::cout << result.ddlData.returnMsg << std::endl;
            }
        }
        printf("ENTER NEW COMMAND\n");
    }
}