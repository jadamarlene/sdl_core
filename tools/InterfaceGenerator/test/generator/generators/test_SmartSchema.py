import sys
import os.path

projectRootDir = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.realpath(__file__)))))
print projectRootDir
sys.path.append(projectRootDir)

import unittest

from generator.generators.SmartSchema import SmartSchema
from generator import Model

expected_result_enum1 = """/**
 * @brief Enumeration Test Name.
 *
 * Description Line1
 * Description Line2
 *
 * Design Line1
 *
 * @note Issue1
 * @note Issue2
 * @note Issue3
 *
 * @todo Do1
 * @todo Do2
 */"""

expected_result_enum_element1 = """/**
 * @brief InternalName.
 */
InternalName = 10"""

expected_result_enum_element2 = """/**
 * @brief NO_VALUE_ELEMENT.
 *
 * Description Line1
 * Description Line2
 *
 * Design Line1
 */
NO_VALUE_ELEMENT"""

description = ["Description Line1", "Description Line2"]
design_description = ["Design Line1"]
issues = ["Issue1", "Issue2", "Issue3"]
todos = ["Do1", "Do2"]
        
class TestSmartSchema(unittest.TestCase):
    
    def test_generate_comment(self):
        smart_schema_generator = SmartSchema()
        

        
        enum = Model.Enum("Test Name", description, design_description, issues, todos)
        self.assertEqual(smart_schema_generator._generate_comment(enum),
                         expected_result_enum1, 
                         "Full comment for enum is invalid")

    def test_generate_enum_element(self):
        smart_schema_generator = SmartSchema()
        
        enum_element1 = Model.EnumElement("Element with internal name",
                                          None, None, None, None, "InternalName", "10")
        self.assertEqual(smart_schema_generator._generate_enum_element(enum_element1),
                         expected_result_enum_element1,
                         "Short commented enum element with internal name is invalid")
        
        enum_element2 = Model.EnumElement("NO_VALUE_ELEMENT",
                                          description, design_description, None,
                                          None, None, None)
        self.assertEqual(smart_schema_generator._generate_enum_element(enum_element2),
                        expected_result_enum_element2,
                        "Enum element with no value is invalid")
        
    #def test_generate_enum_elements(self):
        #smart_schema_generator = SmartSchema()
        
        #elements = [Model.EnumElement("name1", None, design_description, None, todos, None, "1")]
        #enum1 = Model.Enum("Enum Name", description, None, issues, None, None, elements)
        
        #self.assertEqual(smart_schema_generator._generate_enum_elements(enum1),
        #                 expected_result_enum_elements1,
        #                 "Simple enum elements are invalid")

if __name__ == '__main__':
    unittest.main()
