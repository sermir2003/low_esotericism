#include <iostream>
#include <string>
#include <memory>
#include "cmd_args_parser.h"
#include "task.h"
#include "solver.h"

int main(int argc, char* argv[]) {
    std::unique_ptr<InputRequest> request;
    try {
        request = CMDArgsParser::Parse(argc, argv);
    } catch (const ParseException& exception) {
        std::cout << "ERROR: incorrect parameters\n"
                  << exception.what()
                  << "\nYou can get more information using the command 'twineq --help'"
                  << std::endl;
        return 0;
    }
    if (request->GetType() == InputRequestType::CREATE_TASK_FILE) {
        Task::CreateTaskFile(static_cast<RequestCreateTaskFile*>(request.get())->path_task_file);
    }
    else if (request->GetType() == InputRequestType::SOLVE) {
        Task task(static_cast<RequestCreateTaskFile*>(request.get())->path_task_file);
        Solver solver(task, "Search for equilibrium");
        Result result = solver.Solve();
        result.SaveToFile(task.PathResultFile());
    }
    else if (request->GetType() == InputRequestType::HELP) {
        std::cout << "The developer of this program needs help himself.\n"
                     "(function --help not implemented yet)" << std::endl;
    }
    return 0;
}
