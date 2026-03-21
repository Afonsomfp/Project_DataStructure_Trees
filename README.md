The purpose of this project is to explore and compare different search‑tree data structures by applying them to a realistic scenario involving financial transactions.
In this simulated system, each transaction represents an individual financial operation, such as a payment or a bank transfer, and is characterized by a unique identifier,
a monetary value, and a risk level. The project is divided into three phases, each reflecting a fundamental operation commonly found in real financial platforms: insertion,
interval querying, and removal.

In the first phase, a large number of transactions—ranging from one hundred thousand to one million—are inserted into the chosen data structure. Since monetary values are 
not necessarily unique, the structure must be able to store multiple transactions with the same value while maintaining efficient search properties. The second phase 
focuses on interval queries, which are particularly important in real‑world systems used for auditing, analysis, or fraud detection. Each query specifies a range of 
monetary values and a minimum risk level, and the structure must return the identifiers of all transactions that fall within that range. At least ten thousand such queries
must be executed, making this phase the most performance‑critical part of the project. Finally, the third phase involves removing a significant portion of the previously 
inserted transactions. Removals are based solely on the monetary value, and if multiple transactions share that value, all of them must be deleted.
