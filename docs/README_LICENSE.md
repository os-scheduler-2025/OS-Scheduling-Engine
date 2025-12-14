# Preuve d'application de la licence MIT

![MIT License](https://img.shields.io/badge/License-MIT-green.svg)


---

## Licence appliquée

Le projet est distribué sous **MIT License**. 


## Pourquoi le choix de la licence MIT

Le choix de la licence MIT a été motivé par plusieurs raisons :

1. **Simplicité et clarté** : La licence MIT est courte, facile à comprendre et largement reconnue.  
2. **Permissive** : Elle permet à toute personne d’utiliser, modifier et distribuer le projet sans contraintes complexes.  
3. **Compatibilité** : Elle est compatible avec la plupart des autres licences open source, facilitant l’intégration dans d’autres projets.  
4. **Encouragement à la contribution** : Les futurs développeurs peuvent contribuer ou réutiliser le code sans restriction stricte.  

Ce choix assure que le projet reste **libre et ouvert**, tout en protégeant le droit d’auteur de l’auteur du projet.

---


## Vérification de la licence avec ScanCode Toolkit

Pour confirmer que la licence MIT est bien appliquée, le projet a été scanné avec :

- **Outil :** ScanCode Toolkit  
- **Version :** 32.4.1  
- **Commande utilisée :**  

```bash
scancode --license . --json-pp scan_results.json

---

Extrait JSON du rapport ScanCode
{
  "license_detections": [
    {
      "license_expression_spdx": "MIT",
      "from_file": "ordonnanceur/LICENSE",
      "start_line": 1,
      "end_line": 21
    },
    {
      "license_expression_spdx": "MIT",
      "from_file": "ordonnanceur/README.md",
      "start_line": 194,
      "end_line": 194
    }
  ]
}


---


json complet:
{
  "headers": [
    {
      "tool_name": "scancode-toolkit",
      "tool_version": "32.4.1",
      "options": {
        "input": [
          "."
        ],
        "--json-pp": "scan_results.json",
        "--license": true
      },
      "notice": "Generated with ScanCode and provided on an \"AS IS\" BASIS, WITHOUT WARRANTIES\nOR CONDITIONS OF ANY KIND, either express or implied. No content created from\nScanCode should be considered or used as legal advice. Consult an Attorney\nfor any legal advice.\nScanCode is a free software code scanning tool from nexB Inc. and others.\nVisit https://github.com/nexB/scancode-toolkit/ for support and download.",
      "start_timestamp": "2025-12-14T144849.251000",
      "end_timestamp": "2025-12-14T144857.888333",
      "output_format_version": "4.1.0",
      "duration": 8.637362480163574,
      "message": null,
      "errors": [],
      "warnings": [],
      "extra_data": {
        "system_environment": {
          "operating_system": "linux",
          "cpu_architecture": "64",
          "platform": "Linux-4.4.0-26100-Microsoft-x86_64-with-glibc2.35",
          "platform_version": "#5074-Microsoft Fri Jan 01 08:00:00 PST 2016",
          "python_version": "3.10.12 (main, Nov  4 2025, 08:48:33) [GCC 11.4.0]"
        },
        "spdx_license_list_version": "3.27",
        "files_count": 33
      }
    }
  ],
  "license_detections": [
    {
      "identifier": "mit-a7660409-1e42-065e-c739-6d6916c804d7",
      "license_expression": "mit",
      "license_expression_spdx": "MIT",
      "detection_count": 1,
      "reference_matches": [
        {
          "license_expression": "mit",
          "license_expression_spdx": "MIT",
          "from_file": "ordonnanceur/README.md",
          "start_line": 194,
          "end_line": 194,
          "matcher": "2-aho",
          "score": 100.0,
          "matched_length": 2,
          "match_coverage": 100.0,
          "rule_relevance": 100,
          "rule_identifier": "mit_30.RULE",
          "rule_url": "https://github.com/nexB/scancode-toolkit/tree/develop/src/licensedcode/data/rules/mit_30.RULE"
        }
      ]
    },
    {
      "identifier": "mit-ac40beba-5702-f54d-755f-333441314bb0",
      "license_expression": "mit",
      "license_expression_spdx": "MIT",
      "detection_count": 1,
      "reference_matches": [
        {
          "license_expression": "mit",
          "license_expression_spdx": "MIT",
          "from_file": "ordonnanceur/LICENSE",
          "start_line": 1,
          "end_line": 1,
          "matcher": "2-aho",
          "score": 100.0,
          "matched_length": 2,
          "match_coverage": 100.0,
          "rule_relevance": 100,
          "rule_identifier": "mit_14.RULE",
          "rule_url": "https://github.com/nexB/scancode-toolkit/tree/develop/src/licensedcode/data/rules/mit_14.RULE"
        },
        {
          "license_expression": "mit",
          "license_expression_spdx": "MIT",
          "from_file": "ordonnanceur/LICENSE",
          "start_line": 5,
          "end_line": 21,
          "matcher": "2-aho",
          "score": 100.0,
          "matched_length": 161,
          "match_coverage": 100.0,
          "rule_relevance": 100,
          "rule_identifier": "mit.LICENSE",
          "rule_url": "https://github.com/nexB/scancode-toolkit/tree/develop/src/licensedcode/data/licenses/mit.LICENSE"
        }
      ]
    }
  ],
  "files": [
    {
      "path": "ordonnanceur",
      "type": "directory",
      "detected_license_expression": null,
      "detected_license_expression_spdx": null,
      "license_detections": [],
      "license_clues": [],
      "percentage_of_license_text": 0,
      "scan_errors": []
    },
    {
      "path": "ordonnanceur/Installation And User Guide \u2013 Multi-task Scheduler (linux Simulator) (1).pdf",
      "type": "file",
      "detected_license_expression": null,
      "detected_license_expression_spdx": null,
      "license_detections": [],
      "license_clues": [],
      "percentage_of_license_text": 0,
      "scan_errors": []
    },
    {
      "path": "ordonnanceur/LICENSE",
      "type": "file",
      "detected_license_expression": "mit",
      "detected_license_expression_spdx": "MIT",
      "license_detections": [
        {
          "license_expression": "mit",
          "license_expression_spdx": "MIT",
          "matches": [
            {
              "license_expression": "mit",
              "license_expression_spdx": "MIT",
              "from_file": "ordonnanceur/LICENSE",
              "start_line": 1,
              "end_line": 1,
              "matcher": "2-aho",
              "score": 100.0,
              "matched_length": 2,
              "match_coverage": 100.0,
              "rule_relevance": 100,
              "rule_identifier": "mit_14.RULE",
              "rule_url": "https://github.com/nexB/scancode-toolkit/tree/develop/src/licensedcode/data/rules/mit_14.RULE"
            },
            {
              "license_expression": "mit",
              "license_expression_spdx": "MIT",
              "from_file": "ordonnanceur/LICENSE",
              "start_line": 5,
              "end_line": 21,
              "matcher": "2-aho",
              "score": 100.0,
              "matched_length": 161,
              "match_coverage": 100.0,
              "rule_relevance": 100,
              "rule_identifier": "mit.LICENSE",
              "rule_url": "https://github.com/nexB/scancode-toolkit/tree/develop/src/licensedcode/data/licenses/mit.LICENSE"
            }
          ],
          "identifier": "mit-ac40beba-5702-f54d-755f-333441314bb0"
        }
      ],
      "license_clues": [],
      "percentage_of_license_text": 97.02,
      "scan_errors": []
    },
    {
      "path": "ordonnanceur/Makefile",
      "type": "file",
      "detected_license_expression": null,
      "detected_license_expression_spdx": null,
      "license_detections": [],
      "license_clues": [],
      "percentage_of_license_text": 0,
      "scan_errors": []
    },
    {
      "path": "ordonnanceur/ordonnanceur",
      "type": "file",
      "detected_license_expression": null,
      "detected_license_expression_spdx": null,
      "license_detections": [],
      "license_clues": [],
      "percentage_of_license_text": 0,
      "scan_errors": []
    },
    {
      "path": "ordonnanceur/Rapport_technique.md",
      "type": "file",
      "detected_license_expression": null,
      "detected_license_expression_spdx": null,
      "license_detections": [],
      "license_clues": [],
      "percentage_of_license_text": 0,
      "scan_errors": []
    },
    {
      "path": "ordonnanceur/README.md",
      "type": "file",
      "detected_license_expression": "mit",
      "detected_license_expression_spdx": "MIT",
      "license_detections": [
        {
          "license_expression": "mit",
          "license_expression_spdx": "MIT",
          "matches": [
            {
              "license_expression": "mit",
              "license_expression_spdx": "MIT",
              "from_file": "ordonnanceur/README.md",
              "start_line": 194,
              "end_line": 194,
              "matcher": "2-aho",
              "score": 100.0,
              "matched_length": 2,
              "match_coverage": 100.0,
              "rule_relevance": 100,
              "rule_identifier": "mit_30.RULE",
              "rule_url": "https://github.com/nexB/scancode-toolkit/tree/develop/src/licensedcode/data/rules/mit_30.RULE"
            }
          ],
          "identifier": "mit-a7660409-1e42-065e-c739-6d6916c804d7"
        }
      ],
      "license_clues": [],
      "percentage_of_license_text": 0.42,
      "scan_errors": []
    },
    {
      "path": "ordonnanceur/config",
      "type": "directory",
      "detected_license_expression": null,
      "detected_license_expression_spdx": null,
      "license_detections": [],
      "license_clues": [],
      "percentage_of_license_text": 0,
      "scan_errors": []
    },
    {
      "path": "ordonnanceur/config/pr.txt",
      "type": "file",
      "detected_license_expression": null,
      "detected_license_expression_spdx": null,
      "license_detections": [],
      "license_clues": [],
      "percentage_of_license_text": 0,
      "scan_errors": []
    },
    {
      "path": "ordonnanceur/config/processes.txt",
      "type": "file",
      "detected_license_expression": null,
      "detected_license_expression_spdx": null,
      "license_detections": [],
      "license_clues": [],
      "percentage_of_license_text": 0,
      "scan_errors": []
    },
    {
      "path": "ordonnanceur/config/processes7.txt",
      "type": "file",
      "detected_license_expression": null,
      "detected_license_expression_spdx": null,
      "license_detections": [],
      "license_clues": [],
      "percentage_of_license_text": 0,
      "scan_errors": []
    },
    {
      "path": "ordonnanceur/includes",
      "type": "directory",
      "detected_license_expression": null,
      "detected_license_expression_spdx": null,
      "license_detections": [],
      "license_clues": [],
      "percentage_of_license_text": 0,
      "scan_errors": []
    },
    {
      "path": "ordonnanceur/includes/parser.h",
      "type": "file",
      "detected_license_expression": null,
      "detected_license_expression_spdx": null,
      "license_detections": [],
      "license_clues": [],
      "percentage_of_license_text": 0,
      "scan_errors": []
    },
    {
      "path": "ordonnanceur/includes/process.h",
      "type": "file",
      "detected_license_expression": null,
      "detected_license_expression_spdx": null,
      "license_detections": [],
      "license_clues": [],
      "percentage_of_license_text": 0,
      "scan_errors": []
    },
    {
      "path": "ordonnanceur/includes/simulation.h",
      "type": "file",
      "detected_license_expression": null,
      "detected_license_expression_spdx": null,
      "license_detections": [],
      "license_clues": [],
      "percentage_of_license_text": 0,
      "scan_errors": []
    },
    {
      "path": "ordonnanceur/includes/utils.h",
      "type": "file",
      "detected_license_expression": null,
      "detected_license_expression_spdx": null,
      "license_detections": [],
      "license_clues": [],
      "percentage_of_license_text": 0,
      "scan_errors": []
    },
    {
      "path": "ordonnanceur/policies",
      "type": "directory",
      "detected_license_expression": null,
      "detected_license_expression_spdx": null,
      "license_detections": [],
      "license_clues": [],
      "percentage_of_license_text": 0,
      "scan_errors": []
    },
    {
      "path": "ordonnanceur/policies/fifo.c",
      "type": "file",
      "detected_license_expression": null,
      "detected_license_expression_spdx": null,
      "license_detections": [],
      "license_clues": [],
      "percentage_of_license_text": 0,
      "scan_errors": []
    },
    {
      "path": "ordonnanceur/policies/fifo.so",
      "type": "file",
      "detected_license_expression": null,
      "detected_license_expression_spdx": null,
      "license_detections": [],
      "license_clues": [],
      "percentage_of_license_text": 0,
      "scan_errors": []
    },
    {
      "path": "ordonnanceur/policies/mlq.c",
      "type": "file",
      "detected_license_expression": null,
      "detected_license_expression_spdx": null,
      "license_detections": [],
      "license_clues": [],
      "percentage_of_license_text": 0,
      "scan_errors": []
    },
    {
      "path": "ordonnanceur/policies/mlq.so",
      "type": "file",
      "detected_license_expression": null,
      "detected_license_expression_spdx": null,
      "license_detections": [],
      "license_clues": [],
      "percentage_of_license_text": 0,
      "scan_errors": []
    },
    {
      "path": "ordonnanceur/policies/mlq_aging.c",
      "type": "file",
      "detected_license_expression": null,
      "detected_license_expression_spdx": null,
      "license_detections": [],
      "license_clues": [],
      "percentage_of_license_text": 0,
      "scan_errors": []
    },
    {
      "path": "ordonnanceur/policies/mlq_aging.so",
      "type": "file",
      "detected_license_expression": null,
      "detected_license_expression_spdx": null,
      "license_detections": [],
      "license_clues": [],
      "percentage_of_license_text": 0,
      "scan_errors": []
    },
    {
      "path": "ordonnanceur/policies/priority.c",
      "type": "file",
      "detected_license_expression": null,
      "detected_license_expression_spdx": null,
      "license_detections": [],
      "license_clues": [],
      "percentage_of_license_text": 0,
      "scan_errors": []
    },
    {
      "path": "ordonnanceur/policies/priority.so",
      "type": "file",
      "detected_license_expression": null,
      "detected_license_expression_spdx": null,
      "license_detections": [],
      "license_clues": [],
      "percentage_of_license_text": 0,
      "scan_errors": []
    },
    {
      "path": "ordonnanceur/policies/roundrobin.c",
      "type": "file",
      "detected_license_expression": null,
      "detected_license_expression_spdx": null,
      "license_detections": [],
      "license_clues": [],
      "percentage_of_license_text": 0,
      "scan_errors": []
    },
    {
      "path": "ordonnanceur/policies/roundrobin.so",
      "type": "file",
      "detected_license_expression": null,
      "detected_license_expression_spdx": null,
      "license_detections": [],
      "license_clues": [],
      "percentage_of_license_text": 0,
      "scan_errors": []
    },
    {
      "path": "ordonnanceur/src",
      "type": "directory",
      "detected_license_expression": null,
      "detected_license_expression_spdx": null,
      "license_detections": [],
      "license_clues": [],
      "percentage_of_license_text": 0,
      "scan_errors": []
    },
    {
      "path": "ordonnanceur/src/gui.c",
      "type": "file",
      "detected_license_expression": null,
      "detected_license_expression_spdx": null,
      "license_detections": [],
      "license_clues": [],
      "percentage_of_license_text": 0,
      "scan_errors": []
    },
    {
      "path": "ordonnanceur/src/gui.o",
      "type": "file",
      "detected_license_expression": null,
      "detected_license_expression_spdx": null,
      "license_detections": [],
      "license_clues": [],
      "percentage_of_license_text": 0,
      "scan_errors": []
    },
    {
      "path": "ordonnanceur/src/main.c",
      "type": "file",
      "detected_license_expression": null,
      "detected_license_expression_spdx": null,
      "license_detections": [],
      "license_clues": [],
      "percentage_of_license_text": 0,
      "scan_errors": []
    },
    {
      "path": "ordonnanceur/src/main.o",
      "type": "file",
      "detected_license_expression": null,
      "detected_license_expression_spdx": null,
      "license_detections": [],
      "license_clues": [],
      "percentage_of_license_text": 0,
      "scan_errors": []
    },
    {
      "path": "ordonnanceur/src/parser.c",
      "type": "file",
      "detected_license_expression": null,
      "detected_license_expression_spdx": null,
      "license_detections": [],
      "license_clues": [],
      "percentage_of_license_text": 0,
      "scan_errors": []
    },
    {
      "path": "ordonnanceur/src/parser.o",
      "type": "file",
      "detected_license_expression": null,
      "detected_license_expression_spdx": null,
      "license_detections": [],
      "license_clues": [],
      "percentage_of_license_text": 0,
      "scan_errors": []
    },
    {
      "path": "ordonnanceur/src/simulation.c",
      "type": "file",
      "detected_license_expression": null,
      "detected_license_expression_spdx": null,
      "license_detections": [],
      "license_clues": [],
      "percentage_of_license_text": 0,
      "scan_errors": []
    },
    {
      "path": "ordonnanceur/src/simulation.o",
      "type": "file",
      "detected_license_expression": null,
      "detected_license_expression_spdx": null,
      "license_detections": [],
      "license_clues": [],
      "percentage_of_license_text": 0,
      "scan_errors": []
    },
    {
      "path": "ordonnanceur/src/utils.c",
      "type": "file",
      "detected_license_expression": null,
      "detected_license_expression_spdx": null,
      "license_detections": [],
      "license_clues": [],
      "percentage_of_license_text": 0,
      "scan_errors": []
    },
    {
      "path": "ordonnanceur/src/utils.o",
      "type": "file",
      "detected_license_expression": null,
      "detected_license_expression_spdx": null,
      "license_detections": [],
      "license_clues": [],
      "percentage_of_license_text": 0,
      "scan_errors": []
    }
  ]
}
