#include "JSONParser.h"
#include <iostream>
#include <iomanip>

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "JSONParser Test Program" << std::endl;
    std::cout << "========================================" << std::endl;
    
    try {
        // 创建 JSONParser 对象，读取配置文件
        JSONParser parser("input.json");
        
        std::cout << "\n[1] Parsing JSON file: input.json" << std::endl;
        std::cout << "    Status: SUCCESS" << std::endl;
        
        // 打印所有解析的数据
        std::cout << "\n[2] All parsed data:" << std::endl;
        parser.printAll();
        
        // 测试获取各种类型的值
        std::cout << "\n[3] Testing get methods:" << std::endl;
        
        // 维度
        int dim = parser.getInt("dimension");
        std::cout << "    dimension = " << dim << std::endl;
        
        // 网格参数
        int n = parser.getInt("grid.n");
        std::cout << "    grid.n = " << n << std::endl;
        
        // 问题域
        double xmin = parser.getDouble("problem.domain.xmin");
        double xmax = parser.getDouble("problem.domain.xmax");
        double ymin = parser.getDoubleOrDefault("problem.domain.ymin", 0.0);
        double ymax = parser.getDoubleOrDefault("problem.domain.ymax", 1.0);
        std::cout << "    problem.domain.xmin = " << xmin << std::endl;
        std::cout << "    problem.domain.xmax = " << xmax << std::endl;
        std::cout << "    problem.domain.ymin = " << ymin << std::endl;
        std::cout << "    problem.domain.ymax = " << ymax << std::endl;
        
        // 边界条件
        std::string left_type = parser.getStringOrDefault("boundary.left.type", "dirichlet");
        std::string left_value = parser.getStringOrDefault("boundary.left.value", "0");
        std::string right_type = parser.getStringOrDefault("boundary.right.type", "dirichlet");
        std::string right_value = parser.getStringOrDefault("boundary.right.value", "0");
        std::string bottom_type = parser.getStringOrDefault("boundary.bottom.type", "dirichlet");
        std::string bottom_value = parser.getStringOrDefault("boundary.bottom.value", "0");
        std::string top_type = parser.getStringOrDefault("boundary.top.type", "dirichlet");
        std::string top_value = parser.getStringOrDefault("boundary.top.value", "0");
        
        std::cout << "    boundary.left.type = " << left_type << std::endl;
        std::cout << "    boundary.left.value = " << left_value << std::endl;
        std::cout << "    boundary.right.type = " << right_type << std::endl;
        std::cout << "    boundary.right.value = " << right_value << std::endl;
        std::cout << "    boundary.bottom.type = " << bottom_type << std::endl;
        std::cout << "    boundary.bottom.value = " << bottom_value << std::endl;
        std::cout << "    boundary.top.type = " << top_type << std::endl;
        std::cout << "    boundary.top.value = " << top_value << std::endl;
        
        // 测试函数
        std::string test_func_name = parser.getString("test_function.name");
        std::string test_func_exact = parser.getStringOrDefault("test_function.u_exact", "unknown");
        std::cout << "    test_function.name = " << test_func_name << std::endl;
        std::cout << "    test_function.u_exact = " << test_func_exact << std::endl;
        
        // 多重网格参数
        std::string restriction_type = parser.getString("multigrid.restriction");
        std::string interpolation_type = parser.getString("multigrid.interpolation");
        std::string cycle_type = parser.getString("multigrid.cycle");
        int nu1 = parser.getInt("multigrid.nu1");
        int nu2 = parser.getInt("multigrid.nu2");
        double epsilon = parser.getDouble("multigrid.epsilon");
        int max_iter = parser.getInt("multigrid.max_iter");
        
        std::cout << "    multigrid.restriction = " << restriction_type << std::endl;
        std::cout << "    multigrid.interpolation = " << interpolation_type << std::endl;
        std::cout << "    multigrid.cycle = " << cycle_type << std::endl;
        std::cout << "    multigrid.nu1 = " << nu1 << std::endl;
        std::cout << "    multigrid.nu2 = " << nu2 << std::endl;
        std::cout << "    multigrid.epsilon = " << std::scientific << epsilon << std::endl;
        std::cout << "    multigrid.max_iter = " << max_iter << std::endl;
        
        // 输出参数
        bool verbose = parser.getBoolOrDefault("output.verbose", true);
        bool save_solution = parser.getBoolOrDefault("output.save_solution", false);
        std::string output_dir = parser.getStringOrDefault("output.output_dir", "./results");
        
        std::cout << "    output.verbose = " << (verbose ? "true" : "false") << std::endl;
        std::cout << "    output.save_solution = " << (save_solution ? "true" : "false") << std::endl;
        std::cout << "    output.output_dir = " << output_dir << std::endl;
        
        // 测试 hasKey 方法
        std::cout << "\n[4] Testing hasKey method:" << std::endl;
        std::cout << "    hasKey(\"dimension\") = " << (parser.hasKey("dimension") ? "true" : "false") << std::endl;
        std::cout << "    hasKey(\"nonexistent_key\") = " << (parser.hasKey("nonexistent_key") ? "true" : "false") << std::endl;
        
        // 测试默认值
        std::cout << "\n[5] Testing default values:" << std::endl;
        std::string default_test = parser.getStringOrDefault("nonexistent.key", "default_value");
        std::cout << "    getStringOrDefault(\"nonexistent.key\", \"default_value\") = " << default_test << std::endl;
        
        // 测试验证必需键
        std::cout << "\n[6] Testing validateRequiredKeys:" << std::endl;
        std::vector<std::string> required_keys = {"dimension", "grid.n", "multigrid.restriction"};
        parser.validateRequiredKeys(required_keys);
        std::cout << "    All required keys present! ✓" << std::endl;
        
        std::cout << "\n========================================" << std::endl;
        std::cout << "All tests PASSED!" << std::endl;
        std::cout << "========================================" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "\n[ERROR] " << e.what() << std::endl;
        std::cout << "\n========================================" << std::endl;
        std::cout << "Tests FAILED!" << std::endl;
        std::cout << "========================================" << std::endl;
        return 1;
    }
    
    return 0;
}