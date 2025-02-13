# PerformanceEntry Definitions

## Description

The YAML files in this directory are designed to define [PerformanceEntry](https://lynxjs.org/api/lynx-api/performance-api/performance-entry.html). The `generate_performance_entry.py` script utilizes these YAML files to generate corresponding files in Java, Objective-C and TypeScript.

## Usage

- **How to modify a PerformanceEntry?**

_Modifying an Existing PerformanceEntry_: Simply update the relevant YAML file. 😊

_Adding a New PerformanceEntry_: Create a new YAML file and place it in the `lynx/tools/performance/performance_observer/performance_entry_definition_files`. 👷

- **How can I verify my modifications?**

Generate PerformanceEntry files through habitat sync:

```shell
cd /path/to/lynx
source ./tools/envsetup.sh
./hab sync
```

Then check the output:

1. Check `lynx/js_libraries/types/types/background-thread` for TypeScript interfaces.
2. Check `lynx/platform/android/lynx_android/src/main/java/com/lynx/tasm/performance/performanceobserver` for Java files.
3. Check `lynx/platform/darwin/common/lynx/performance/performance_observer` for Objective-C implementation files and `lynx/platform/darwin/common/lynx/public/performance/performance_observer` for Objective-C header files.

### YAML Syntax

A PerformanceEntry file should include a new class name, inheritances, and variable definitions. Its structure typically looks like this:

```yaml
# NewEntry.yml
NewEntry:
  allOf:
    - $ref: 'PerformanceEntry.yml#/PerformanceEntry'
    - type: object
      properties:
        propName:
            type: string
```

`allOf` signifies that your definition inherits from `PerformanceEntry`, so ensure that `NewEntry` maintains this inheritance relationship.

You can define basic data types as follows:

```yaml
name:
    type: number/string/string/map
```

For more complex object types, use:

```yaml
name:
    $ref: 'ReferenceObject.yml#/ReferenceObject'
```

#### Extension Tags

To effectively manage multi-platform outputs, we've introduced the `x-ts-only` symbol to control the generation of TypeScript scripts exclusively. Additionally, custom symbols can be added as needed.

For example, by using the `x-ts-only` tag in NewEntry.yml, **only NewEntry.ts** will be generated, without producing scripts for Java, C++, or any other languages.

```yaml
# NewEntry.yml
NewEntry:
  x-ts-only: true
  allOf:
    - $ref: 'PerformanceEntry.yml#/PerformanceEntry'
    - type: object
      properties:
        propName:
            type: string
```
