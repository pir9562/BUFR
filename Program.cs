using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Reflection;

namespace pivot_unpivot
{

     
    class Program
    {
        //Helper method to make the Select cleaner:
        private static string GetProperty(IEnumerable<DataRow> rows, string propertyName)
        {
            return rows
                .Where(row => row.Field<string>("Property") == propertyName)
                .Select(c => c.Field<string>("Value"))
                .FirstOrDefault();
        }

        //helper method for populating the datatable
        private static void addRow(DataTable dt, string objectName, string columnName
            , string property, string value)
        {
            var row = dt.NewRow();
            row["ObjectName"] = objectName;
            row["ColumnName"] = columnName;
            row["Property"] = property;
            row["Value"] = value;
            dt.Rows.Add(row);
        }

        public static void Main(string[] args)
        {

            DataTable dt = new DataTable();
            dt.Columns.Add("ObjectName");
            dt.Columns.Add("ColumnName");
            dt.Columns.Add("Property");
            dt.Columns.Add("Value");

            addRow(dt, "foo", "bar", "a", "w");
            addRow(dt, "foo", "bar", "b", "x");
            addRow(dt, "foo", "bar", "c", "y");
            addRow(dt, "foo", "bar", "d", "z");
            //addRow(dt, "foo", "test", "a", "i");
            addRow(dt, "foo", "test", "b", "j");
            addRow(dt, "foo", "test", "c", "k");
            addRow(dt, "foo", "test", "d", "l");
            addRow(dt, "foo", "test", "e", "u");

            var query = dt.AsEnumerable()
                .GroupBy(row => new
                {
                    ObjectName = row.Field<string>("ObjectName"),
                    ColumnName = row.Field<string>("ColumnName")
                })
                .Select(g => new
                {
                    ObjectName = g.Key.ObjectName,
                    ColumnName = g.Key.ColumnName,
                    b = GetProperty(g, "b"),
                    c = GetProperty(g, "c"),
                    d = GetProperty(g, "d"),
                    a = GetProperty(g, "a"),
                    e = GetProperty(g, "e")
                })
                .CopyToDataTable();

            foreach( DataColumn column in query.Columns) {
                System.Console.Write(column.ColumnName + "\t");
               
            }
            System.Console.WriteLine();
           

            foreach (DataRow row in query.Rows)
            {
                
                foreach (DataColumn column in query.Columns)
                {
                    System.Console.Write(row[column] + "\t");
                }
                System.Console.WriteLine();
            }


            
            Console.WriteLine("Press any key to exit. . .");
            Console.ReadKey(true);

        }
    }


    /// <summary>
    /// Code copied directly from http://msdn.microsoft.com/en-us/library/bb669096.aspx
    /// </summary>
    /// <typeparam name="T"></typeparam>
    public class PivotConverter<T>
    {
        private System.Reflection.FieldInfo[] _fieldInfo;
        private System.Reflection.PropertyInfo[] _propertyInfo;
        private System.Collections.Generic.Dictionary<string, int> columnMap;
        private System.Type _type;

        // PivotConverter constructor.
        public PivotConverter()
        {
            _type = typeof(T);
            _fieldInfo = _type.GetFields();
            _propertyInfo = _type.GetProperties();
            columnMap = new Dictionary<string, int>();
        }

        /// <summary>
        /// Loads a DataTable from a sequence of objects.
        /// </summary>
        /// <param name="source">The sequence of objects to load into the DataTable.</param>
        /// <param name="table">The input table. The schema of the table must match that 
        /// the type T.  If the table is null, a new table is created with a schema 
        /// created from the public properties and fields of the type T.</param>
        /// <param name="options">Specifies how values from the source sequence will be applied to 
        /// existing rows in the table.</param>
        /// <returns>A DataTable created from the source sequence.</returns>
        public DataTable Shred(IEnumerable<T> source, DataTable table, LoadOption? options)
        {
            // Load the table from the scalar sequence if T is a primitive type.
            if (typeof(T).IsPrimitive)
            {
                return SetSourcePrimitiveIntoTarget(source, table, options);
            }

            // Create a new table if the input table is null.
            if (table == null)
            {
                table = new DataTable(typeof(T).Name);
            }

            // Initialize the ordinal map and extend the table schema based on type T.
            table = ExtendTable(table, typeof(T));

            // Enumerate the source sequence and load the object values into rows.
            table.BeginLoadData();
            using (IEnumerator<T> e = source.GetEnumerator())
            {
                while (e.MoveNext())
                {
                    if (options != null)
                    {
                        table.LoadDataRow(GetFieldAndPropertyInfosFromSource(table, e.Current), (LoadOption)options);
                    }
                    else
                    {
                        table.LoadDataRow(GetFieldAndPropertyInfosFromSource(table, e.Current), true);
                    }
                }
            }
            table.EndLoadData();

            // Return the table.
            return table;
        }

        public DataTable SetSourcePrimitiveIntoTarget(IEnumerable<T> source, DataTable table, LoadOption? options)
        {
            // Create a new table if the input table is null.
            if (table == null)
            {
                table = new DataTable(typeof(T).Name);
            }

            if (!table.Columns.Contains("Value"))
            {
                table.Columns.Add("Value", typeof(T));
            }

            // Enumerate the source sequence and load the scalar values into rows.
            table.BeginLoadData();
            using (IEnumerator<T> e = source.GetEnumerator())
            {
                Object[] values = new object[table.Columns.Count];
                while (e.MoveNext())
                {
                    values[table.Columns["Value"].Ordinal] = e.Current;

                    if (options != null)
                    {
                        table.LoadDataRow(values, (LoadOption)options);
                    }
                    else
                    {
                        table.LoadDataRow(values, true);
                    }
                }
            }
            table.EndLoadData();

            // Return the table.
            return table;
        }

        public object[] GetFieldAndPropertyInfosFromSource(DataTable table, T instance)
        {

            FieldInfo[] fi = _fieldInfo;
            PropertyInfo[] pi = _propertyInfo;

            if (instance.GetType() != typeof(T))
            {
                // If the instance is derived from T, extend the table schema
                // and get the properties and fields.
                ExtendTable(table, instance.GetType());
                fi = instance.GetType().GetFields();
                pi = instance.GetType().GetProperties();
            }

            // Add the property and field values of the instance to an array.
            Object[] values = new object[table.Columns.Count];
            foreach (FieldInfo f in fi)
            {
                values[columnMap[f.Name]] = f.GetValue(instance);
            }

            foreach (PropertyInfo p in pi)
            {
                values[columnMap[p.Name]] = p.GetValue(instance, null);
            }

            // Return the property and field values of the instance.
            return values;
        }

        public DataTable ExtendTable(DataTable table, Type type)
        {
            // Extend the table schema if the input table was null or if the value 
            // in the sequence is derived from type T.            
            foreach (FieldInfo f in type.GetFields())
            {
                // 맵이 ColumnName을 가지고 있지 않다면
                if (!columnMap.ContainsKey(f.Name))
                {
                    // 테이블(table)에 Column이 존재하면 해당 DataColumn객체를 받고
                    // 테이블에 Column이 존재하지 않으면 테이블에 이름과 필드타입을 가지는 컬럼을 생성하고 DataColumn객체를 받음
                    DataColumn dc = table.Columns.Contains(f.Name) ? table.Columns[f.Name] : table.Columns.Add(f.Name, f.FieldType);

                    // Map에 Column(열)의 위치(Ordinal)와 열의이름(ColumnName)을 저장
                    columnMap.Add(f.Name, dc.Ordinal);
                }
            }
            foreach (PropertyInfo p in type.GetProperties())
            {
                if (!columnMap.ContainsKey(p.Name))
                {
                    // Add the property as a column in the table if it doesn't exist
                    // already.
                    DataColumn dc = table.Columns.Contains(p.Name) ? table.Columns[p.Name]
                        : table.Columns.Add(p.Name, p.PropertyType);

                    // Add the property to the ordinal map.
                    columnMap.Add(p.Name, dc.Ordinal);
                }
            }

            // Return the table.
            return table;
        }
    }

    /// <summary>
    /// Code copied directly from http://msdn.microsoft.com/en-us/library/bb669096.aspx
    /// </summary>
    public static class CustomLINQtoDataSetMethods
    {
        public static DataTable CopyToDataTable<T>(this IEnumerable<T> source)
        {
            return new PivotConverter<T>().Shred(source, null, null);
        }

        public static DataTable CopyToDataTable<T>(this IEnumerable<T> source,
                                                    DataTable table, LoadOption? options)
        {
            return new PivotConverter<T>().Shred(source, table, options);
        }

    }
}
